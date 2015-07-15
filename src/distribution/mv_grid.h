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

/**
 *  This module contains definition of the mv_grid interface.
 */

namespace cst {

/**
 *  Multivariate grid interface provides a grid of according dimensions used to
 *  make continuous objects discrete.
 */
class mv_grid_i {
public:
  /**
   *  Set underlying grid directly, which is vector of vectors.
   *
   *  @param grid Grid to set as current.
   */
  virtual void set_grid(const std::vector<vec_t>& grid) const { _grid = grid; }

  /**
   *  Set underlying grid to uniform grid/
   *
   *  @param grid_begin  Upper left grid corner.
   *  @param grid_end    Lower right grid corner.
   *  @param grid_detail Grid resolution on each side.
   */
  virtual void set_grid(const vec_t& grid_begin, const vec_t& grid_end,
                        size_t grid_detail) const {
    _grid.clear();
    size_t dim = grid_begin.size();
    vec_t steps(dim);

    for (size_t k = 0; k < dim; ++k) {
      steps[k] = (grid_end[k] - grid_begin[k]) / grid_detail;
    }

    rec_grid(grid_begin, grid_end, steps, grid_detail, 0, dim, vec_t());
  }
  virtual const std::vector<vec_t>& grid() const { return _grid; }

protected:
  /**
   *  Recursive subroutine to model dynamic number of nested loops.
   */
  void rec_grid(const vec_t& grid_begin, const vec_t& grid_end,
                const vec_t& steps, size_t grid_detail, size_t coordinate,
                size_t dim, vec_t point) const {
    point.push_back(0);
    for (size_t i = 0; i < grid_detail + 1; ++i) {
      point[coordinate] = grid_begin[coordinate] + i * steps[coordinate];
      if (coordinate + 1 < dim) {
        rec_grid(grid_begin, grid_end, steps, grid_detail, coordinate + 1, dim,
                 point);
      } else {
        _grid.push_back(point);
      }
    }
  }
  mutable std::vector<vec_t> _grid;
};
}

#endif
