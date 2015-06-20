//
//  e_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_e_distribution_h
#define corrstat_e_distribution_h

#include <algorithm>
#include <cmath>
#include <fstream>

#include "distribution_i.h"
#include "../result.h"

namespace cst {

class e_distribution_t : public distribution_t {
public:
  e_distribution_t() {}
  e_distribution_t(const vec_t& sample)
      : _sample_size(sample.size()), _sample(sample) {
    std::sort(_sample.begin(), _sample.end());
  }

  virtual num_t prob(num_t x) const { return 0; }

  virtual num_t density(num_t x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t cdf(num_t x) const {
    num_t num = 0;
    for (size_t i = 0; i < _sample.size(); ++i) {
      if (_sample[i] <= x) {
        num += 1;
      }
    }
    return num / _sample.size();
  }

  virtual num_t inv_cdf(num_t p) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t mean() const {
    num_t acc = 0;
    for (size_t i = 0; i < _sample.size(); ++i) {
      acc += _sample[i];
    }
    return acc / _sample.size();
  }

  virtual num_t variance() const {
    num_t acc = 0;
    num_t m = mean();
    for (size_t i = 0; i < _sample.size(); ++i) {
      acc += (_sample[i] - m) * (_sample[i] - m);
    }
    return acc / (_sample.size() - 1);
  }

  virtual num_t supp_lower_bound() const {
    num_t min = INFINITY;
    for (size_t i = 0; i < _sample.size(); ++i) {
      if (_sample[i] < min) {
        min = _sample[i];
      }
    }
    return min;
  }

  virtual num_t supp_upper_bound() const {
    num_t max = -INFINITY;
    for (size_t i = 0; i < _sample.size(); ++i) {
      if (max < _sample[i]) {
        max = _sample[i];
      }
    }
    return max;
  }

  virtual bool is_supp_lower_bound_inclusive() const { return true; }
  virtual bool is_supp_upper_bound_inclusive() const { return true; }

  virtual num_t sample() const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  static result<e_distribution_t> read(std::string path_to_data) {
    std::fstream stream;
    stream.open(path_to_data.c_str());

    if (stream.fail()) {
      std::string err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<e_distribution_t>::error(err_text, error_t::io_error);
    }

    vec_t sample;
    num_t element;
    while (stream >> element) {
      sample.push_back(element);
    }

    bool reached_eof = stream.eof();
    stream.close();

    if (reached_eof) {
      e_distribution_t d(sample);
      return result<e_distribution_t>::ok(d);
    }

    return result<e_distribution_t>::error("File contains invalid number.",
                                           error_t::io_error);
  }

  size_t sample_size() const { return _sample_size; }

private:
  size_t _sample_size;
  vec_t _sample;
};
}

#endif
