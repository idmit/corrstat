//
//  hope.cpp
//  corrstat
//
//  Created by Ivan Dmitrievsky on 23/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "distribution/e_distribution.h"
#include "distribution/emv_distribution.h"
#include "distribution/gaussian_copula.h"
#include "hope.h"

#include "csv.h"

#include "nlopt/nlopt.hpp"

template <typename T> std::string to_string(T num) {
  std::ostringstream ss;
  ss << num;
  return ss.str();
}

namespace cst {

struct max_data {
  const emv_distribution_t *emv_dist;
  const e_distribution_t **e_dists;
};

cst::num_t max_func(const vec_t &x, vec_t &grad, void *data) {
  max_data *mdata = reinterpret_cast<max_data *>(data);
  const emv_distribution_t *emv_dist = mdata->emv_dist;
  const e_distribution_t **e_dists = mdata->e_dists;

  gaussian_copula_t g_cop(2, x[0]);

  num_t acc = 0;
  for (size_t i = 0; i < emv_dist->sample_size(); ++i) {
    vec_t element = emv_dist->sample_at(i);
    vec_t cop_args(2);
    cop_args[0] = e_dists[0]->cdf(element[0]);
    cop_args[1] = e_dists[1]->cdf(element[1]);
    num_t density = g_cop.density(cop_args);
    num_t d1 = e_dists[0]->density(element[0]);
    num_t d2 = e_dists[1]->density(element[1]);
    //    printf("%lf : %lf : %lf : %lf\n", x[0], density, d1, d2);
    acc += std::log(density) + std::log(d1) + std::log(d2);
  }

  return acc;
}

hope_t::hope_t() {}

hope_t::hope_t(const std::vector<vec_t> &model_vals,
               const std::vector<vec_t> &bounds,
               const std::vector<vec_t> &hit_numbers)
    : _model_vals(model_vals), _bounds(bounds), _hit_numbers(hit_numbers) {}

result<hope_t> hope_t::make(const std::string &model_vals_filename,
                            const std::string &bounds_filename,
                            const std::string &hit_numbers_filename) {

  std::vector<vec_t> model_vals;
  //  result<void *> load_models = load_csv(model_vals_filename, model_vals, 6);
  //  if (!load_models) {
  //    return result<hope_t>::error(load_models.err());
  //  }

  std::vector<vec_t> bounds_vals;
  //  result<void *> load_bounds = load_csv(bounds_filename, bounds_vals, 12);
  //  if (!load_bounds) {
  //    return result<hope_t>::error(load_bounds.err());
  //  }

  std::vector<vec_t> hit_numbers;
  //  result<void *> load_hit_numbers =
  //      load_csv(hit_numbers_filename, hit_numbers, 7);
  //  if (!load_hit_numbers) {
  //    return result<hope_t>::error(load_hit_numbers.err());
  //  }

  return result<hope_t>::ok(hope_t(model_vals, bounds_vals, hit_numbers));
}

result<void *> hope_t::export_middles(const std::string &path_to_write) const {
  std::vector<vec_t> middles(_bounds.size());
  for (size_t i = 0; i < _bounds.size(); ++i) {
    if (_bounds[i].size() > 0) {
      for (size_t k = 0; k < 6; ++k) {
        middles[i].push_back(_bounds[i][2 * k] +
                             (_bounds[i][2 * k + 1] - _bounds[i][2 * k]) / 2);
      }
    }
  }
  return save_csv(path_to_write, middles, 6);
}

result<void *> hope_t::export_unique_model_vals(
    const std::string &base_path_to_write) const {
  std::vector<vec_t> unique_model_vals(_model_vals[0].size());

  for (size_t i = 0; i < unique_model_vals.size(); ++i) {
    for (size_t k = 0; k < _model_vals.size(); ++k) {

      if (std::find(unique_model_vals[i].begin(), unique_model_vals[i].end(),
                    _model_vals[k][i]) == unique_model_vals[i].end()) {
        unique_model_vals[i].push_back(_model_vals[k][i]);
      }
    }
  }

  std::vector<vec_t> tmp(1);
  for (size_t i = 0; i < unique_model_vals.size(); ++i) {
    tmp[0] = unique_model_vals[i];
    result<void *> saved = save_csv(base_path_to_write + to_string(i) + ".csv",
                                    tmp, tmp[0].size());
    if (!saved) {
      return saved;
    }
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_shifts(const std::string &path_to_middles,
                                     const std::string &path_to_write) const {
  std::vector<vec_t> middles;

  result<void *> loaded = load_csv(path_to_middles, middles, 6);
  if (!loaded) {
    return loaded;
  }

  std::vector<vec_t> shifts(middles.size());
  size_t rows_per_model = middles.size() / _model_vals.size();

  for (size_t j = 0; j < _model_vals.size(); ++j) {
    for (size_t i = 0; i < rows_per_model; ++i) {
      if (middles[j * rows_per_model + i].size() > 0) {
        for (size_t k = 0; k < 6; ++k) {
          shifts[j * rows_per_model + i].push_back(
              _model_vals[j][k] - middles[j * rows_per_model + i][k]);
        }
      }
    }
  }

  return save_csv(path_to_write, shifts, 6);
}

result<void *> hope_t::export_marginals(
    const std::string &path_to_shifts, const std::string &base_path_to_read,
    const std::string &path_to_write) const {
  size_t params = _model_vals[0].size();
  std::vector<vec_t> unique_model_vals(_model_vals[0].size());

  for (size_t i = 0; i < params; ++i) {
    std::vector<vec_t> tmp_unique_values;

    result<void *> loaded = load_csv(base_path_to_read + to_string(i) + ".csv",
                                     tmp_unique_values, 0, true);
    if (!loaded) {
      return loaded;
    }
    unique_model_vals[i] = tmp_unique_values[0];
  }
  std::vector<vec_t> shifts;

  result<void *> loaded = load_csv(path_to_shifts, shifts, 6);
  if (!loaded) {
    return loaded;
  }

  const char *trace_lengths[] = { "100", "250", "1000" };

  for (size_t i = 0; i < params; ++i) {
    for (size_t j = 0; j < unique_model_vals[i].size(); ++j) {
      std::vector<vec_t> margin(3);

      for (size_t k = 0; k < shifts.size(); ++k) {
        if (_model_vals[k / 300][i] == unique_model_vals[i][j]) {
          if (shifts[k].size() > 0) {
            margin[(k % 300) / 100].push_back(shifts[k][i]);
          }
        }
      }
      for (size_t k = 0; k < 3; ++k) {
        std::ofstream stream;
        stream.open((path_to_write + '_' + to_string(i) + '_' + to_string(j) +
                     '_' + trace_lengths[k])
                        .c_str(),
                    std::ofstream::trunc);

        if (stream.fail()) {
          std::string err_text =
              path_to_write + std::string(": ") + strerror(errno);
          return result<void *>::error(err_text, error_t::io_error);
        }

        for (size_t n = 0; n < margin[k].size(); ++n) {
          stream << margin[k][n] << '\n';
        }
        stream.close();

        system(
            (std::string(
                 "gnuplot -e "
                 "\"filename='/Users/ivandmi/Documents/dev/corrstat/corrstat/"
                 "data/shift_marginals/margin_") +
             to_string(i) + "_" + to_string(j) + "_" + trace_lengths[k] +
             "'\" -e "
             "\"plotname='/Users/ivandmi/Documents/dev/corrstat/corrstat/"
             "data/shift_marginals/plot_" +
             to_string(i) + "_" + to_string(j) + "_" + trace_lengths[k] +
             ".png'\" "
             "/Users/ivandmi/Documents/dev/corrstat/corrstat/data/"
             "shift_marginals/run.gp")
                .c_str());
      }
    }
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_joint(const std::string &path_to_shifts,
                                    const std::string &base_path_to_read,
                                    const size_t params_and_vals[4],
                                    const std::string &path_to_write) const {
  size_t params = _model_vals[0].size();
  std::vector<vec_t> unique_model_vals(_model_vals[0].size());

  for (size_t i = 0; i < params; ++i) {
    std::vector<vec_t> tmp_unique_values;

    result<void *> loaded = load_csv(base_path_to_read + to_string(i) + ".csv",
                                     tmp_unique_values, 0, true);
    if (!loaded) {
      return loaded;
    }
    unique_model_vals[i] = tmp_unique_values[0];
  }
  std::vector<vec_t> shifts;

  result<void *> loaded = load_csv(path_to_shifts, shifts, 6);
  if (!loaded) {
    return loaded;
  }

  const char *trace_lengths[] = { "100", "250", "1000" };
  std::vector<std::vector<vec_t> > joint(3, std::vector<vec_t>(2));

  for (size_t k = 0; k < shifts.size(); ++k) {
    if (_model_vals[k / 300][params_and_vals[0]] ==
            unique_model_vals[params_and_vals[0]][params_and_vals[1]] &&
        _model_vals[k / 300][params_and_vals[2]] ==
            unique_model_vals[params_and_vals[2]][params_and_vals[3]]) {
      if (shifts[k].size() > 0) {
        joint[(k % 300) / 100][0].push_back(shifts[k][params_and_vals[0]]);
        joint[(k % 300) / 100][1].push_back(shifts[k][params_and_vals[2]]);
      }
    }
  }

  for (size_t k = 0; k < 3; ++k) {
    std::ofstream stream;
    stream.open((path_to_write + '_' + to_string(params_and_vals[0]) +
                 to_string(params_and_vals[1]) + '_' +
                 to_string(params_and_vals[2]) + to_string(params_and_vals[3]) +
                 '_' + trace_lengths[k])
                    .c_str(),
                std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text =
          path_to_write + std::string(": ") + strerror(errno);
      return result<void *>::error(err_text, error_t::io_error);
    }

    for (size_t n = 0; n < joint[k][0].size(); ++n) {
      stream << joint[k][0][n] << ' ' << joint[k][1][n] << '\n';
    }
    stream.close();

    system((std::string(
                "gnuplot -e "
                "\"filename='/Users/ivandmi/Documents/dev/corrstat/corrstat/"
                "data/shift_joints/joint_") +
            to_string(params_and_vals[0]) + to_string(params_and_vals[1]) +
            "_" + to_string(params_and_vals[2]) +
            to_string(params_and_vals[3]) + "_" + trace_lengths[k] +
            "'\" -e "
            "\"plotname='/Users/ivandmi/Documents/dev/corrstat/corrstat/"
            "data/shift_joints/scatter_" +
            to_string(params_and_vals[0]) + to_string(params_and_vals[1]) +
            "_" + to_string(params_and_vals[2]) +
            to_string(params_and_vals[3]) + "_" + trace_lengths[k] +
            ".png'\" "
            "/Users/ivandmi/Documents/dev/corrstat/corrstat/data/"
            "shift_joints/scatter.gp")
               .c_str());
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_estimations(
    const std::string &path_to_margins, const std::string &path_to_joints,
    const std::string &base_path_to_read, const size_t params_and_vals[4],
    const std::string &path_to_write) const {
  size_t params = 6;
  std::vector<vec_t> unique_model_vals(6);

  for (size_t i = 0; i < params; ++i) {
    std::vector<vec_t> tmp_unique_values;

    result<void *> loaded = load_csv(base_path_to_read + to_string(i) + ".csv",
                                     tmp_unique_values, 0, true);
    if (!loaded) {
      return loaded;
    }
    unique_model_vals[i] = tmp_unique_values[0];
  }

  const char *trace_lengths[] = { "100", "250", "1000" };

  for (size_t k = 0; k < 3; ++k) {
    std::ofstream stream;
    stream.open((path_to_write + '_' + to_string(params_and_vals[0]) +
                 to_string(params_and_vals[1]) + '_' +
                 to_string(params_and_vals[2]) + to_string(params_and_vals[3]) +
                 '_' + trace_lengths[k])
                    .c_str(),
                std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text =
          path_to_write + std::string(": ") + strerror(errno);
      return result<void *>::error(err_text, error_t::io_error);
    }

    const e_distribution_t *margins[2];

    result<e_distribution_t> loaded_1 = e_distribution_t::read(
        path_to_margins + '_' + to_string(params_and_vals[0]) + '_' +
        to_string(params_and_vals[1]) + '_' + trace_lengths[k]);
    if (!loaded_1) {
      return result<void *>::error(loaded_1.err());
    }

    result<e_distribution_t> loaded_2 = e_distribution_t::read(
        path_to_margins + '_' + to_string(params_and_vals[2]) + '_' +
        to_string(params_and_vals[3]) + '_' + trace_lengths[k]);
    if (!loaded_2) {
      return result<void *>::error(loaded_2.err());
    }

    margins[0] = loaded_1.borrow();
    margins[1] = loaded_2.borrow();

    result<emv_distribution_t> loaded_3 = emv_distribution_t::read(
        path_to_joints + '_' + to_string(params_and_vals[0]) +
        to_string(params_and_vals[1]) + "_" + to_string(params_and_vals[2]) +
        to_string(params_and_vals[3]) + "_" + trace_lengths[k]);

    if (!loaded_3) {
      return result<void *>::error(loaded_3.err());
    }

    const emv_distribution_t *joint = loaded_3.borrow();

    max_data mdata = { joint, margins };

    nlopt::opt opt(nlopt::LN_COBYLA, 1);

    std::vector<double> lb(1);
    lb[0] = -1;
    opt.set_lower_bounds(lb);
    std::vector<double> ub(1);
    ub[0] = 1;
    opt.set_upper_bounds(ub);

    opt.set_max_objective(max_func, &mdata);

    opt.set_xtol_rel(1e-4);

    std::vector<double> x(1);
    x[0] = 0;
    double maxf;
    nlopt::result result = opt.optimize(x, maxf);

    if (result < 0) {
      printf("nlopt failed!\n");
    } else {
      printf("found maximum at f(%g) = %0.10g\n", x[0], maxf);
    }

    //    size_t m = 50;
    //    num_t d = 2.0 / 50;
    //    vec_t grad;
    //    for (size_t n = 1; n < m; ++n) {
    //      vec_t arg(1, -1 + d * n);
    //      stream << -1 + d *n << ' ' << max_func(arg, grad, &mdata) << '\n';
    //    }

    stream.close();
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::save_csv(const std::string &filename,
                                const std::vector<vec_t> &values,
                                size_t row_size) {
  std::ofstream stream;
  stream.open(filename.c_str(), std::ofstream::trunc);

  if (stream.fail()) {
    std::string err_text = filename + std::string(": ") + strerror(errno);
    return result<void *>::error(err_text, error_t::io_error);
  }

  for (size_t i = 0; i < values.size(); ++i) {
    for (size_t k = 0; k < row_size; ++k) {
      if (values[i].size() > 0) {
        stream << values[i][k] << ';';
      } else {
        stream << ';';
      }
    }
    stream << '\n';
  }
  stream.close();
  return result<void *>::ok(NULL);
}

result<void *> hope_t::load_csv(const std::string &filename,
                                std::vector<vec_t> &values, size_t row_size,
                                bool dynamic) {
  std::fstream stream;
  stream.open(filename.c_str());

  std::string err_text;

  if (stream.fail()) {
    err_text = filename + std::string(": ") + strerror(errno);
    return result<void *>::error(err_text, error_t::io_error);
  }
  for (CSVIterator row(stream); row != CSVIterator(); ++row) {
    if (dynamic) {
      row_size = row->size();
    }
    if (row->size() != row_size) {
      std::string err_text =
          filename + std::string(": ") + "row size if wrong.";
      return result<void *>::error(err_text, error_t::io_error);
    }
    values.push_back(vec_t());
    for (size_t k = 0; k < row_size; ++k) {
      if ((*row)[k] != "") {
        values.back().push_back(atof((*row)[k].c_str()));
      }
    }
  }

  bool reached_eof = stream.eof();
  stream.close();

  if (!reached_eof) {
    err_text = filename + std::string(": ") + "Couldn't reach end of file.";
    return result<void *>::error(err_text, error_t::io_error);
  }

  return result<void *>::ok(NULL);
}
}
