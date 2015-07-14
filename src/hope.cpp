//
//  hope.cpp
//  corrstat
//
//  Created by Ivan Dmitrievsky on 23/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "distribution/e_distribution.h"
#include "distribution/emv_distribution.h"
#include "distribution/gaussian_copula.h"
#include "distribution/e_copula.h"
#include "hope.h"

#include "csv.h"

template <typename T> std::string to_string(T num) {
  std::ostringstream ss;
  ss << num;
  return ss.str();
}

namespace cst {

result<void *> hope_t::read_model_vals(const std::string &model_vals_filename,
                                       std::vector<vec_t> &model_vals) {
  result<void *> load_models = load_csv(model_vals_filename, model_vals, 6);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }
  return result<void *>::ok(NULL);
}

result<void *> hope_t::read_bounds(const std::string &bounds_filename,
                                   std::vector<vec_t> &bounds) {
  result<void *> load_bounds = load_csv(bounds_filename, bounds, 12);
  if (!load_bounds) {
    return result<void *>::error(load_bounds.err());
  }
  return result<void *>::ok(NULL);
}

result<void *> hope_t::read_hit_vals(const std::string &hit_numbers_filename,
                                     std::vector<vec_t> &hit_numbers) {
  result<void *> load_hit_numbers =
      load_csv(hit_numbers_filename, hit_numbers, 7);
  if (!load_hit_numbers) {
    return result<void *>::error(load_hit_numbers.err());
  }
  return result<void *>::ok(NULL);
}

result<void *> hope_t::read_unique_model_vals(
    const std::string &base_path_to_read,
    std::vector<vec_t> &unique_model_vals) {

  for (size_t i = 0; i < 6; ++i) {
    std::vector<vec_t> tmp_unique_values;

    result<void *> loaded = load_csv(base_path_to_read + to_string(i) + ".csv",
                                     tmp_unique_values, 0, true);
    if (!loaded) {
      return loaded;
    }
    unique_model_vals.push_back(tmp_unique_values[0]);
  }
  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_middles(const std::string &bounds_filename,
                                      const std::string &path_to_write) const {
  std::vector<vec_t> bounds;
  result<void *> load_bounds = read_bounds(bounds_filename, bounds);
  if (!load_bounds) {
    return result<void *>::error(load_bounds.err());
  }

  std::vector<vec_t> middles(bounds.size());
  for (size_t i = 0; i < bounds.size(); ++i) {
    if (bounds[i].size() > 0) {
      for (size_t k = 0; k < 6; ++k) {
        middles[i].push_back(bounds[i][2 * k] +
                             (bounds[i][2 * k + 1] - bounds[i][2 * k]) / 2);
      }
    }
  }

  return save_csv(path_to_write, middles, 6);
}

result<void *> hope_t::export_unique_model_vals(
    const std::string &model_vals_filename,
    const std::string &base_path_to_write) const {
  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  std::vector<vec_t> unique_model_vals(model_vals[0].size());

  for (size_t i = 0; i < unique_model_vals.size(); ++i) {
    for (size_t k = 0; k < model_vals.size(); ++k) {

      if (std::find(unique_model_vals[i].begin(), unique_model_vals[i].end(),
                    model_vals[k][i]) == unique_model_vals[i].end()) {
        unique_model_vals[i].push_back(model_vals[k][i]);
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

result<void *> hope_t::export_shifts(const std::string &model_vals_filename,
                                     const std::string &path_to_middles,
                                     const std::string &path_to_write) const {
  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  std::vector<vec_t> middles;
  result<void *> loaded = load_csv(path_to_middles, middles, 6);
  if (!loaded) {
    return loaded;
  }

  std::vector<vec_t> shifts(middles.size());
  size_t rows_per_model = middles.size() / model_vals.size();

  for (size_t j = 0; j < model_vals.size(); ++j) {
    for (size_t i = 0; i < rows_per_model; ++i) {
      if (middles[j * rows_per_model + i].size() > 0) {
        for (size_t k = 0; k < 6; ++k) {
          shifts[j * rows_per_model + i].push_back(
              model_vals[j][k] - middles[j * rows_per_model + i][k]);
        }
      }
    }
  }

  return save_csv(path_to_write, shifts, 6);
}

result<void *> hope_t::export_hit_marginals(
    const std::string &model_vals_filename,
    const std::string &path_to_hit_numbers,
    const std::string &base_path_to_read,
    const std::string &path_to_write) const {

  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  size_t params = model_vals[0].size();
  std::vector<vec_t> unique_model_vals;
  result<void *> loaded =
      read_unique_model_vals(base_path_to_read, unique_model_vals);
  if (!loaded) {
    return loaded;
  }

  std::vector<vec_t> hits;
  loaded = load_csv(path_to_hit_numbers, hits, 6);
  if (!loaded) {
    return loaded;
  }

  const char *trace_lengths[] = { "100", "250", "1000" };

  for (size_t i = 0; i < params; ++i) {
    for (size_t j = 0; j < unique_model_vals[i].size(); ++j) {
      std::vector<vec_t> margin(3);

      for (size_t k = 0; k < hits.size(); ++k) {
        if (model_vals[k / 300][i] == unique_model_vals[i][j]) {
          if (hits[k].size() > 0) {
            margin[(k % 300) / 100].push_back(hits[k][i]);
          }
        }
      }
      for (size_t k = 0; k < 3; ++k) {
        std::ofstream stream;
        stream.open((path_to_write + "/margin_" + to_string(i) + '_' +
                     to_string(j) + '_' + trace_lengths[k])
                        .c_str(),
                    std::ofstream::trunc);

        if (stream.fail()) {
          std::string err_text =
              path_to_write + std::string(": ") + strerror(errno);
          return result<void *>::error(err_text, error_t::io_error);
        }

        num_t prob = 0;
        for (size_t n = 0; n < margin[k].size(); ++n) {
          prob += margin[k][n];
        }
        prob /= margin[k].size();

        stream << margin[k].size() << ';' << prob << '\n';
        stream.close();
      }
    }
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_marginals(
    const std::string &model_vals_filename, const std::string &path_to_shifts,
    const std::string &base_path_to_read,
    const std::string &path_to_write) const {

  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  size_t params = model_vals[0].size();
  std::vector<vec_t> unique_model_vals;
  read_unique_model_vals(base_path_to_read, unique_model_vals);

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
        if (model_vals[k / 300][i] == unique_model_vals[i][j]) {
          if (shifts[k].size() > 0) {
            margin[(k % 300) / 100].push_back(shifts[k][i]);
          }
        }
      }
      for (size_t k = 0; k < 3; ++k) {
        std::ofstream stream;
        std::string dest_path = path_to_write + "/margin_" + to_string(i) +
                                '_' + to_string(j) + '_' + trace_lengths[k];
        std::string plot_path = path_to_write + "/plot_" + to_string(i) + '_' +
                                to_string(j) + '_' + trace_lengths[k];
        stream.open(dest_path.c_str(), std::ofstream::trunc);

        if (stream.fail()) {
          std::string err_text =
              path_to_write + std::string(": ") + strerror(errno);
          return result<void *>::error(err_text, error_t::io_error);
        }

        for (size_t n = 0; n < margin[k].size(); ++n) {
          stream << margin[k][n] << '\n';
        }
        stream.close();

        //        gnuplot(dest_path, plot_path, path_to_write + "/run.gp");
      }
    }
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_joint(const std::string &model_vals_filename,
                                    const std::string &path_to_shifts,
                                    const std::string &base_path_to_read,
                                    const size_t params_and_vals[4],
                                    const std::string &path_to_write) const {
  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  std::vector<vec_t> unique_model_vals;
  result<void *> loaded =
      read_unique_model_vals(base_path_to_read, unique_model_vals);
  if (!loaded) {
    return loaded;
  }

  std::vector<vec_t> shifts;
  loaded = load_csv(path_to_shifts, shifts, 6);
  if (!loaded) {
    return loaded;
  }

  const char *trace_lengths[] = { "100", "250", "1000" };
  std::vector<std::vector<vec_t> > joint(3, std::vector<vec_t>(2));

  for (size_t k = 0; k < shifts.size(); ++k) {
    if (model_vals[k / 300][params_and_vals[0]] ==
            unique_model_vals[params_and_vals[0]][params_and_vals[1]] &&
        model_vals[k / 300][params_and_vals[2]] ==
            unique_model_vals[params_and_vals[2]][params_and_vals[3]]) {
      if (shifts[k].size() > 0) {
        joint[(k % 300) / 100][0].push_back(shifts[k][params_and_vals[0]]);
        joint[(k % 300) / 100][1].push_back(shifts[k][params_and_vals[2]]);
      }
    }
  }

  for (size_t k = 0; k < 3; ++k) {
    std::ofstream stream;
    std::string dest_path =
        path_to_write + "/joint_" + to_string(params_and_vals[0]) +
        to_string(params_and_vals[1]) + '_' + to_string(params_and_vals[2]) +
        to_string(params_and_vals[3]) + '_' + trace_lengths[k];
    std::string scatter_path =
        path_to_write + "/scatter_" + to_string(params_and_vals[0]) +
        to_string(params_and_vals[1]) + '_' + to_string(params_and_vals[2]) +
        to_string(params_and_vals[3]) + '_' + trace_lengths[k];
    stream.open(dest_path.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text =
          path_to_write + std::string(": ") + strerror(errno);
      return result<void *>::error(err_text, error_t::io_error);
    }

    for (size_t n = 0; n < joint[k][0].size(); ++n) {
      stream << joint[k][0][n] << ' ' << joint[k][1][n] << '\n';
    }
    stream.close();

    //    gnuplot(dest_path, scatter_path, path_to_write + "/scatter.gp");
  }

  return result<void *>::ok(NULL);
}

result<void *> hope_t::export_hit_joint(
    const std::string &model_vals_filename, const std::string &path_to_hits,
    const std::string &base_path_to_read,
    const std::string &path_to_write) const {
  std::vector<vec_t> model_vals;
  result<void *> load_models = read_model_vals(model_vals_filename, model_vals);
  if (!load_models) {
    return result<void *>::error(load_models.err());
  }

  size_t params = model_vals[0].size();
  std::vector<vec_t> unique_model_vals;
  result<void *> loaded =
      read_unique_model_vals(base_path_to_read, unique_model_vals);
  if (!loaded) {
    return loaded;
  }

  std::vector<vec_t> hits;
  loaded = load_csv(path_to_hits, hits, 6);
  if (!loaded) {
    return loaded;
  }

  const char *trace_lengths[] = { "100", "250", "1000" };

  for (size_t i = 0; i < params - 1; ++i) {
    for (size_t j = 0; j < unique_model_vals[i].size(); ++j) {
      for (size_t x = i + 1; x < params; ++x) {
        for (size_t y = 0; y < unique_model_vals[x].size(); ++y) {

          std::vector<std::vector<vec_t> > joint(3, std::vector<vec_t>(2));

          for (size_t k = 0; k < hits.size(); ++k) {
            if (model_vals[k / 300][i] == unique_model_vals[i][j] &&
                model_vals[k / 300][x] == unique_model_vals[x][y]) {
              if (hits[k].size() > 0) {
                joint[(k % 300) / 100][0].push_back(hits[k][i]);
                joint[(k % 300) / 100][1].push_back(hits[k][x]);
              }
            }
          }

          for (size_t k = 0; k < 3; ++k) {
            std::ofstream stream;
            std::string dest_path = path_to_write + "/joint_" + to_string(i) +
                                    to_string(j) + '_' + to_string(x) +
                                    to_string(y) + '_' + trace_lengths[k];
            std::string cop_path = path_to_write + "/e-cop_" + to_string(i) +
                                   to_string(j) + '_' + to_string(x) +
                                   to_string(y) + '_' + trace_lengths[k];
            std::string plot_path = path_to_write + "/plot_" + to_string(i) +
                                    to_string(j) + '_' + to_string(x) +
                                    to_string(y) + '_' + trace_lengths[k];
            std::string scatter_path =
                path_to_write + "/scatter_" + to_string(i) + to_string(j) +
                '_' + to_string(x) + to_string(y) + '_' + trace_lengths[k];
            stream.open(dest_path.c_str(), std::ofstream::trunc);

            if (stream.fail()) {
              std::string err_text =
                  path_to_write + std::string(": ") + strerror(errno);
              return result<void *>::error(err_text, error_t::io_error);
            }

            std::vector<vec_t> joint_p;
            size_t window = 30;

            for (size_t s = 0; s < joint[k][0].size() - window + 1; ++s) {
              vec_t p(2, 0);
              for (size_t t = 0; t < window; ++t) {
                p[0] += joint[k][0][s + t];
                p[1] += joint[k][1][s + t];
              }
              p[0] /= window;
              p[1] /= window;
              joint_p.push_back(p);
            }

            for (size_t n = 0; n < joint_p.size(); ++n) {
              stream << joint_p[n][0] << ' ' << joint_p[n][1];
              if (n < joint_p.size() - 1) {
                stream << '\n';
              }
            }
            stream.close();

            result<emv_distribution_t> dist =
                emv_distribution_t::read(dest_path);

            if (!dist) {
              return result<void *>::error(dist.err());
            }
            dist->set_grid(vec_t(2, 0), vec_t(2, 1), 30);

            e_copula_t e_cop(dist.borrow());
            e_cop.set_grid(vec_t(2, 0.01), vec_t(2, 0.99), 20);
            e_cop.export_density(cop_path);

            gnuplot(cop_path, plot_path, path_to_write + "/run.gp", i, x,
                    unique_model_vals[i][j], unique_model_vals[x][y]);
            //            gnuplot(dest_path, scatter_path, path_to_write +
            //            "/scatter.gp");
          }
        }
      }
    }
  }
  return result<void *>::ok(NULL);
}

void hope_t::gnuplot(const std::string &filename, const std::string &plotname,
                     const std::string &scriptname, size_t x, size_t y,
                     num_t xval, num_t yval) {
  const char *estimate_names[] = {
    "\\widehat{p}_\\mu",     "\\widehat{p}_\\sigma",
    "\\widehat{p}_\\lambda", "\\widehat{p}_{\\lambda_c}",
    "\\widehat{p}_{x_m}",    "\\widehat{p}_\\alpha"

  };
  const char *param_names[] = {
    "\\mu", "\\sigma", "\\lambda", "\\lambda_c", "x_m", "\\alpha",
  };

  std::string cmd = "gnuplot ";
  cmd += "-e \"filename='" + filename + "'\" ";
  cmd += "-e \"plotname='" + plotname + ".tex'\" ";
  cmd += "-e \"vxlabel='$" + std::string(estimate_names[x]) + "$'\" ";
  cmd += "-e \"vylabel='$" + std::string(estimate_names[y]) + "$'\" ";
  cmd += "-e \"vtitle='$" + std::string(param_names[x]) + "=" +
         to_string(xval) + "$, " + "$" + std::string(param_names[y]) + "=" +
         to_string(yval) + "$'\" ";

  cmd += scriptname;
  system(cmd.c_str());
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
    if (row->size() < row_size) {
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
