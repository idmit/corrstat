//
//  mv_grid.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 18/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_mv_grid_h
#define corrstat_mv_grid_h

#include "../types.h"

namespace cst {

class mv_grid_i {
public:
  virtual void set_grid(const std::vector<vec_t>& grid) const { _grid = grid; }
  virtual void set_grid(const vec_t& grid_begin, const vec_t& grid_end,
                        const size_t grid_detail) const {
    _grid.clear();
    size_t dim = grid_begin.size();
    vec_t steps(dim);

    for (size_t k = 0; k < dim; ++k) {
      steps[k] = (grid_end[k] - grid_begin[k]) / grid_detail;
    }

    // FIXME: This is grid is only diagonal
    for (size_t i = 0; i < grid_detail; ++i) {
      vec_t tmp(dim);
      for (size_t k = 0; k < dim; ++k) {
        tmp[k] = grid_begin[k] + i * steps[k];
      }
    }
  }
  virtual const std::vector<vec_t>& grid() const { return _grid; }

protected:
  mutable std::vector<vec_t> _grid;
};
}

#endif
