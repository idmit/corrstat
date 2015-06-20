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

#include "mv_distribution_i.h"

namespace cst {

class emv_distribution_t : public mv_distribution_t {
public:
  emv_distribution_t() : mv_distribution_t(0) {}
  emv_distribution_t(const std::vector<vec_t>& mv_sample)
      : mv_distribution_t(mv_sample[0].size()),
        _sample_size(mv_sample.size()),
        _mv_sample(mv_sample) {

    std::sort(_mv_sample.begin(), _mv_sample.end(), less);
  }

  virtual num_t density(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t cdf(const vec_t& x) const {
    num_t num = 0;
    for (size_t i = 0; i < _sample_size; ++i) {
      bool ok = true;
      for (size_t k = 0; k < _dim; ++k) {
        if (_mv_sample[i][k] > x[k]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        num += 1;
      }
    }
    return num / _sample_size;
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

  static result<emv_distribution_t> read(std::string path_to_data) {
    std::fstream stream;
    stream.open(path_to_data.c_str());

    std::string err_text;

    if (stream.fail()) {
      err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<emv_distribution_t>::error(err_text, error_t::io_error);
    }

    std::vector<vec_t> mv_sample;
    num_t element;
    std::string line;
    std::stringstream line_stream;

    if (std::getline(stream, line)) {
      line_stream.str(line);
      vec_t sample;
      while (line_stream >> element) {
        sample.push_back(element);
      }
      mv_sample.push_back(sample);
    }

    while (std::getline(stream, line)) {
      line_stream.str(line);
      line_stream.clear();
      vec_t sample;
      mv_sample.push_back(sample);
      size_t i = 0;
      while (line_stream >> element) {
        if (i < mv_sample[0].size()) {
          mv_sample.back().push_back(element);
          ++i;
        } else {
          err_text = path_to_data + std::string(": ") +
                     "Some row has more columns than the first one.";
          return result<emv_distribution_t>::error(err_text, error_t::io_error);
        }
      }
      if (i < mv_sample[0].size()) {
        err_text = path_to_data + std::string(": ") +
                   "Some row has less columns than the first one.";
        return result<emv_distribution_t>::error(err_text, error_t::io_error);
      }
    }

    bool reached_eof = stream.eof();
    stream.close();

    if (reached_eof) {
      emv_distribution_t d(mv_sample);
      return result<emv_distribution_t>::ok(d);
    }

    err_text =
        path_to_data + std::string(": ") + "File contains invalid number.";
    return result<emv_distribution_t>::error(err_text, error_t::io_error);
  }

  size_t sample_size() const { return _sample_size; }

  virtual void set_sample_as_grid() const { _grid = _mv_sample; }

protected:
  size_t _sample_size;
  std::vector<vec_t> _mv_sample;
};
}

#endif
