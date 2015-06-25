//
//  gaussian_copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 25/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_gaussian_copula_h
#define corrstat_gaussian_copula_h

#include <cmath>

#include "copula.h"

#include "normal_distribution.h"

namespace cst {

class gaussian_copula_t : public copula_t {
public:
  gaussian_copula_t(size_t dim, num_t rho)
      : copula_t(dim), _rho(rho), _normal(0, 1) {}

  virtual num_t call(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t density(const vec_t& x) const {
    num_t nu = _normal.inv_cdf(x[0]);
    num_t nv = _normal.inv_cdf(x[1]);
    num_t rho2 = _rho * _rho;
    return std::exp((2 * nu * nv * _rho - rho2 * (nu * nu - nv * nv)) /
                    (2 * (1 - rho2))) /
           (2 * PI * (1 - rho2));
  }

private:
  num_t _rho;
  normal_distribution_t _normal;
};
}

#endif
