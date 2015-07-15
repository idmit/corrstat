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

#include <boost/random/uniform_real_distribution.hpp>

namespace cst {

class uniform_distribution_t : public distribution_t {
public:
  uniform_distribution_t(num_t lower, num_t upper)
      : _lower(lower), _upper(upper), _dist(lower, upper) {}

  virtual num_t density(num_t x) const {
    if (x < _lower || _upper < x) {
      return 0;
    }
    return 1 / (_upper - _lower);
  }

  virtual num_t cdf(num_t x) const {
    if (x <= _lower) {
      return 0;
    }
    if (_upper <= x) {
      return 1;
    }
    return (x - _lower) / (_upper - _lower);
  }

  virtual num_t inv_cdf(num_t p) const {
    assert(p < 0 || 1 < p);
    return _lower + p * (_upper - _lower);
  }

  virtual num_t mean() const { return (_lower + _upper) / 2; }

  virtual num_t variance() const {
    double d = _upper - _lower;
    return d * d / 12;
  }

  virtual num_t supp_lower_bound() const { return _lower; }
  virtual num_t supp_upper_bound() const { return _upper; }

  virtual bool is_supp_lower_bound_inclusive() const { return true; }
  virtual bool is_supp_upper_bound_inclusive() const { return true; }

  virtual num_t sample() const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

private:
  num_t _lower, _upper;
  boost::random::uniform_real_distribution<num_t> _dist;
};
}

#endif
