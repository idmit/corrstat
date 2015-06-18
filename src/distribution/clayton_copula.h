//
//  clayton_copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 13/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_clayton_copula_h
#define corrstat_clayton_copula_h

#include <cmath>

#include "acopula.h"

namespace cst {

class clayton_copula_t : public acopula_t {
public:
  clayton_copula_t(size_t dim, num_t theta) : acopula_t(dim), _theta(theta) {
    assert(-1 <= theta && theta != 0 && theta < INFINITY);
  }

  virtual num_t gen(num_t x) const {
    return (std::pow(x, -_theta) - 1) / _theta;
  }

  virtual num_t inv_gen(num_t x) const {
    return std::pow(_theta * x + 1, -1 / _theta);
  }

private:
  num_t _theta;
};
}

#endif
