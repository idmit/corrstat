//
//  frank_copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 13/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_frank_copula_h
#define corrstat_frank_copula_h

#include <cmath>

#include "acopula.h"

namespace cst {

class frank_copula_t : public acopula_t {
public:
  frank_copula_t(size_t dim, num_t theta) : acopula_t(dim), _theta(theta) {
    assert(theta != 0);
  }

  virtual num_t gen(num_t x) const {
    return -std::log((std::exp(-_theta * x) - 1) / (std::exp(-_theta) - 1));
  }

  virtual num_t inv_gen(num_t x) const {
    return -std::log(1 + std::exp(-x) * (std::exp(-_theta) - 1)) / _theta;
  }

private:
  num_t _theta;
};
}

#endif
