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

class e_distribution_t : public distribution_i {
public:
  e_distribution_t() {}
  e_distribution_t(const vec_t& samples) : _samples(samples) {
    std::sort(_samples.begin(), _samples.end());
  }

  virtual num_t prob(num_t x) const { return 0; }

  virtual num_t density(num_t x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t cdf(num_t x) const {
    num_t num = 0;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (_samples[i] <= x) {
        num += 1;
      }
    }
    return num / _samples.size();
  }

  virtual num_t mean() const {
    num_t acc = 0;
    for (size_t i = 0; i < _samples.size(); ++i) {
      acc += _samples[i];
    }
    return acc / _samples.size();
  }

  virtual num_t variance() const {
    num_t acc = 0;
    num_t m = mean();
    for (size_t i = 0; i < _samples.size(); ++i) {
      acc += (_samples[i] - m) * (_samples[i] - m);
    }
    return acc / (_samples.size() - 1);
  }

  virtual num_t supp_lower_bound() const {
    num_t min = INFINITY;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (_samples[i] < min) {
        min = _samples[i];
      }
    }
    return min;
  }

  virtual num_t supp_upper_bound() const {
    num_t max = -INFINITY;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (max < _samples[i]) {
        max = _samples[i];
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

    vec_t samples;
    num_t sample;
    while (stream >> sample) {
      samples.push_back(sample);
    }

    bool reached_eof = stream.eof();
    stream.close();

    if (reached_eof) {
      e_distribution_t d(samples);
      return result<e_distribution_t>::ok(d);
    }

    return result<e_distribution_t>::error("File contains invalid number.",
                                           error_t::io_error);
  }

  void export_cdf(std::string path_to_data) {
    std::fstream stream;
    stream.open(path_to_data.c_str());

    vec_t samples;
    for (size_t i = 0; i < _samples.size(); ++i) {
      stream << _samples[i] << ' ' << cdf(_samples[i]) << '\n';
    }
    stream.close();
  }

private:
  vec_t _samples;
};
}

#endif
