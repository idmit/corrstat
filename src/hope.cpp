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

#include "hope.h"

#include "csv.h"

template <typename T> std::string to_string(T num) {
  std::ostringstream ss;
  ss << num;
  return ss.str();
}

namespace cst {
hope_t::hope_t() {}

hope_t::hope_t(const std::vector<vec_t> &model_vals,
               const std::vector<vec_t> &bounds,
               const std::vector<vec_t> &hit_numbers)
    : _model_vals(model_vals), _bounds(bounds), _hit_numbers(hit_numbers) {}

result<hope_t> hope_t::make(const std::string &model_vals_filename,
                            const std::string &bounds_filename,
                            const std::string &hit_numbers_filename) {

  std::vector<vec_t> model_vals;
  result<void *> load_models = load_csv(model_vals_filename, model_vals, 6);
  if (!load_models) {
    return result<hope_t>::error(load_models.err());
  }

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
                     '_' + to_string(std::pow(10, k + 1)))
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
             to_string(i) + "_" + to_string(j) + "_" +
             to_string(std::pow(10, k + 1)) +
             "'\" -e "
             "\"plotname='/Users/ivandmi/Documents/dev/corrstat/corrstat/"
             "data/shift_marginals/plot_" +
             to_string(i) + "_" + to_string(j) + "_" +
             to_string(std::pow(10, k + 1)) +
             ".png'\" "
             "/Users/ivandmi/Documents/dev/corrstat/corrstat/data/"
             "shift_marginals/run.gp")
                .c_str());
      }
    }
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
