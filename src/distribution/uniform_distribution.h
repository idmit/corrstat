//
//  uniform_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 15/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_uniform_distribution_h
#define corrstat_uniform_distribution_h

#include <cmath>

#include "distribution.h"

namespace cst {

class uniform_distribution_t : public distribution_t {
  uniform_distribution_t(num_t lower, num_t upper)
      : _lower(lower), _upper(upper) {}

  virtual num_t density(num_t x) {
    if (x < _lower || _upper < x) {
      return 0;
    }
    return 1 / (_upper - _lower);
  }

  virtual num_t cdf(num_t x) {
    if (x <= _lower) {
      return 0;
    }
    if (_upper <= x) {
      return 1;
    }
    return (x - _lower) / (_upper - _lower);
  }

  virtual num_t inv_cdf(num_t p) {
    assert(p < 0 || 1 < p);
    return _lower + p * (_upper - _lower);
  }

  virtual num_t mean() { return (_lower + _upper) / 2; }

  virtual num_t variance() {
    double d = _upper - _lower;
    return d * d / 12;
  }

  virtual num_t supp_lower_bound() { return _lower; }
  virtual num_t supp_upper_bound() { return _upper; }

  virtual bool is_supp_lower_bound_inclusive() { return true; }
  virtual bool is_supp_upper_bound_inclusive() { return true; }

  virtual num_t sample() { return 0; }

private:
  num_t _lower, _upper;
};
}

#endif
