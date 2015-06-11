//
//  invcopula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_invcopula_h
#define corrstat_invcopula_h

#include "copula.h"
#include "mv_distribution.h"

#include "shared_ptr.hpp"

namespace cst {

class invcopula_t : public copula_t {
public:
  invcopula_t(mv_distribution_t *mv_dist, const std::vector<fn> &inverses)
      : copula_t(mv_dist->dim()), _mv_dist(mv_dist), _inverses(inverses) {}

  virtual num_t call(vec_t x) {
    vec_t cp(_dim);
    for (size_t i = 0; i < _dim; ++i) {
      cp[i] = _inverses[i](x[i]);
    }

    return _mv_dist->cdf(cp);
  }

private:
  shared_ptr<mv_distribution_t> _mv_dist;
  std::vector<fn> _inverses;
};
}

#endif
