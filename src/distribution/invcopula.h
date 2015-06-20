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
#include "distribution.h"
#include "mv_distribution.h"

namespace cst {

class invcopula_t : public copula_t {
public:
  invcopula_t(const mv_distribution_t *mv_dist, const std::vector<fn> &inverses)
      : copula_t(mv_dist->dim()),
        _mv_dist(mv_dist),
        _plain_inverses(true),
        _inverses(inverses) {}

  invcopula_t(const mv_distribution_t *mv_dist,
              const std::vector<const distribution_t *> &uv_dists)
      : copula_t(mv_dist->dim()),
        _mv_dist(mv_dist),
        _plain_inverses(false),
        _inverses(),
        _uv_dists(uv_dists) {}

  virtual num_t call(const vec_t &x) {
    vec_t cp(_dim);

    if (_plain_inverses) {
      for (size_t i = 0; i < _dim; ++i) {
        cp[i] = _inverses[i](x[i]);
      }
    } else {
      for (size_t i = 0; i < _dim; ++i) {
        cp[i] = _uv_dists[i]->inv_cdf(x[i]);
      }
    }

    return _mv_dist->cdf(cp);
  }

  virtual num_t density(const vec_t &x) {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

private:
  const mv_distribution_t *_mv_dist;
  bool _plain_inverses;
  std::vector<fn> _inverses;
  std::vector<const distribution_t *> _uv_dists;
};
}

#endif
