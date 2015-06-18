//
//  emv_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 11/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_emv_distribution_h
#define corrstat_emv_distribution_h

#include <algorithm>
#include <fstream>
#include <sstream>

#include "mv_distribution_i.h"

namespace cst {

class emv_distribution_t : public mv_distribution_i {
public:
  emv_distribution_t() {}
  emv_distribution_t(const std::vector<vec_t> &mv_samples)
      : _dim(mv_samples[0].size()), _mv_samples(mv_samples) {

    std::sort(_mv_samples.begin(), _mv_samples.end(), less);
  }

  virtual num_t density(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t cdf(const vec_t& x) const {
    num_t num = 0;
    for (size_t i = 0; i < _mv_samples.size(); ++i) {
      bool ok = true;
      for (size_t k = 0; k < _dim; ++k) {
        if (_mv_samples[i][k] > x[k]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        num += 1;
      }
    }
    return num / _mv_samples.size();
  }

  virtual size_t dim() const { return _dim; }
  virtual vec_t sample() const {
    vec_t s(1);
    s[0] = 0;
    return s;
  }

  virtual num_t margin_cdf(size_t coordinate, num_t x) const {
    num_t num = 0;
    for (size_t i = 0; i < _sample_size; ++i) {
      if (_mv_sample[i][coordinate] <= x) {
        num += 1;
      }
    }
    return num / (_sample_size + 1);
  }

  virtual void calc_margin_cdfs_on_grid() const {
    for (size_t i = 0; i < _sample_size; ++i) {
      vec_t tmp(_dim);
      _margin_cdfs_on_grid.push_back(tmp);
      for (size_t coordinate = 0; coordinate < _dim; ++coordinate) {
        _margin_cdfs_on_grid[i][coordinate] =
            margin_cdf(coordinate, _mv_sample[i][coordinate]);
      }
    }
  }

  virtual num_t margin_cdf_on_grid(size_t coordinate,
                                   size_t element_num) const {
    return _margin_cdfs_on_grid[element_num][coordinate];
  }

  static result<emv_distribution_t> read(std::string path_to_data) {
    std::fstream stream;
    stream.open(path_to_data.c_str());

    std::vector<vec_t> mv_samples;
    num_t sample;
    std::string line;
    std::stringstream line_stream;

    if (std::getline(stream, line)) {
      line_stream.str(line);
      vec_t samples;
      while (line_stream >> sample) {
        samples.push_back(sample);
      }
      mv_samples.push_back(samples);
    }

    while (std::getline(stream, line)) {
      line_stream.str(line);
      line_stream.clear();
      vec_t samples;
      mv_samples.push_back(samples);
      size_t i = 0;
      while (line_stream >> sample) {
        if (i < mv_samples[0].size()) {
          mv_samples.back().push_back(sample);
          ++i;
        } else {
          return result<emv_distribution_t>::error(
              "Some row has more columns than the first one.",
              error_t::io_error);
        }
      }
      if (i < mv_samples[0].size()) {
        return result<emv_distribution_t>::error(
            "Some row has less columns than the first one.", error_t::io_error);
      }
    }

    bool reached_eof = stream.eof();
    stream.close();

    if (reached_eof) {
      emv_distribution_t d(mv_samples);
      return result<emv_distribution_t>::ok(d);
    }

    return result<emv_distribution_t>::error("File contains invalid number.",
                                             error_t::io_error);
  }

  void export_cdf(std::string path_to_data) {
    std::fstream stream;
    stream.open(path_to_data.c_str());

    vec_t samples;
    for (size_t i = 0; i < _mv_samples.size(); ++i) {
      for (size_t k = 0; k < _dim; ++k) {
        stream << _mv_samples[i][k] << ' ';
      }
      vec_t x;
      for (size_t k = 0; k < _dim; ++k) {
        x.push_back(_mv_samples[i][k]);
      }
      stream << cdf(x) << '\n';
    }
    stream.close();
  }

protected:
  size_t _dim;
  std::vector<vec_t> _mv_samples;
  mutable std::vector<vec_t> _margin_cdfs_on_grid;
};
}

#endif
