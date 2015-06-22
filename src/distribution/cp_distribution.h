//
//  cp_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_cp_distribution_h
#define corrstat_cp_distribution_h

#include "copula.h"
#include "distribution.h"
#include "mv_distribution.h"

namespace cst {

class cp_distribution_t : public mv_distribution_t {
public:
  cp_distribution_t(const copula_t* copula,
                    const std::vector<const distribution_t*>& margins)
      : mv_distribution_t(copula->dim()), _copula(copula), _margins(margins) {
    assert(copula->dim() == margins.size());
  }

  virtual num_t density(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t cdf(const vec_t& x) const {
    vec_t cp(_dim);
    for (size_t i = 0; i < _dim; ++i) {
      cp[i] = _margins[i]->cdf(x[i]);
    }

    return _copula->call(cp);
  }

  virtual num_t margin_cdf(size_t coordinate, num_t x) const {
    // TODO: Implement if needed.
    // These margins are not affected by copula. It's wrong.
    assert(false);
    return _margins[coordinate]->cdf(x);
  }

  void set_tied_grid() {
    _tied_grid.clear();
    std::vector<vec_t> margins;

    for (size_t i = 0; i < _margins.size(); ++i) {
      vec_t uv;
      for (size_t k = 0; k < _margins.size(); ++k) {
        uv.push_back(_margins[i]->grid()[k][0]);
      }
      margins.push_back(uv);
    }

    rec_dist(margins, 0, _margins.size(), vec_t());
  }

  void set_tied_grid_as_grid() { _grid = _tied_grid; }

  virtual vec_t sample() const {
    // TODO: Implement if needed.
    assert(false);
    return vec_t(1, 0);
  }

private:
  void rec_dist(const std::vector<vec_t>& margins, size_t coordinate,
                size_t dim, vec_t point) const {
    point.push_back(0);
    for (size_t i = 0; i < margins[coordinate].size(); ++i) {
      point[coordinate] = margins[coordinate][i];
      if (coordinate + 1 < dim) {
        rec_dist(margins, coordinate + 1, dim, point);
      } else {
        _tied_grid.push_back(point);
      }
    }
  }

  const copula_t* _copula;
  mutable std::vector<vec_t> _tied_grid;
  std::vector<const distribution_t*> _margins;
};
}

#endif
