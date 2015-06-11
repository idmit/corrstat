//
//  e_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_e_distribution_h
#define corrstat_e_distribution_h

#include "distribution_i.h"

namespace cst {

class e_distribution_t : public distribution_i {
public:
  e_distribution_t(const vec_t& samples) : _samples(samples) {}

  virtual num_t prob(num_t x) { return 0; }

  virtual num_t density(num_t x) { return 0; }

  virtual num_t cdf(num_t x) {
    num_t num = 0;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (_samples[i] <= x) {
        num += 1;
      }
    }
    return num / _samples.size();
  }

  virtual num_t mean() {
    num_t acc = 0;
    for (size_t i = 0; i < _samples.size(); ++i) {
      acc += _samples[i];
    }
    return acc / _samples.size();
  }

  virtual num_t variance() {
    num_t acc = 0;
    num_t m = mean();
    for (size_t i = 0; i < _samples.size(); ++i) {
      acc += (_samples[i] - m) * (_samples[i] - m);
    }
    return acc / (_samples.size() - 1);
  }

  virtual num_t supp_lower_bound() {
    num_t min = INFINITY;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (_samples[i] < min) {
        min = _samples[i];
      }
    }
    return min;
  }

  virtual num_t supp_upper_bound() {
    num_t max = -INFINITY;
    for (size_t i = 0; i < _samples.size(); ++i) {
      if (max < _samples[i]) {
        max = _samples[i];
      }
    }
    return max;
  }

  virtual bool is_supp_lower_bound_inclusive() { return true; }
  virtual bool is_supp_upper_bound_inclusive() { return true; }

  virtual num_t sample() { return 0; }

private:
  vec_t _samples;
};
}

#endif
