//
//  gumbel_copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_gumbel_copula_h
#define corrstat_gumbel_copula_h

#include <cmath>

#include "acopula.h"

namespace cst {

class gumbel_copula_t : public acopula_t {
public:
  gumbel_copula_t(size_t dim, num_t theta) : acopula_t(dim), _theta(theta) {
    assert(1 <= theta && theta < INFINITY);
  }

  virtual num_t gen(num_t x) const { return std::pow(-std::log(x), _theta); }

  virtual num_t inv_gen(num_t x) const {
    return std::exp(-std::pow(x, 1 / _theta));
  }

private:
  num_t _theta;
};
}

#endif
