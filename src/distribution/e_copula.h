//
//  e_copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 17/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_e_copula_h
#define corrstat_e_copula_h

#include <cerrno>

#include "copula.h"
#include "distribution.h"
#include "kernel.h"
#include "emv_distribution.h"

#include "shared_ptr.hpp"

namespace cst {

class e_copula_t : public copula_i {
public:
  e_copula_t(const emv_distribution_t* mv_dist)
      : _dim(mv_dist->dim()), _mv_dist(mv_dist) {}

  virtual num_t call(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t operator()(const vec_t& x) const { return call(x); }

  virtual num_t mirror_density(const vec_t& x, gaussian_ker ker,
                               num_t h = 0.5) const {
    num_t acc = 0;

    _mv_dist->calc_margin_cdfs_on_grid();

    for (size_t i = 0; i < sample_size(); ++i) {
      num_t U = _mv_dist->margin_cdf_on_grid(0, i);
      num_t V = _mv_dist->margin_cdf_on_grid(1, i);

      acc += ker((x[0] - U) / h) * ker((x[1] - V) / h) +
             ker((x[0] + U) / h) * ker((x[1] - V) / h) +
             ker((x[0] - U) / h) * ker((x[1] + V) / h) +
             ker((x[0] + U) / h) * ker((x[1] + V) / h) +
             ker((x[0] - U) / h) * ker((x[1] - 2 + V) / h) +
             ker((x[0] + U) / h) * ker((x[1] - 2 + V) / h) +
             ker((x[0] - 2 + U) / h) * ker((x[1] - V) / h) +
             ker((x[0] - 2 + U) / h) * ker((x[1] + V) / h) +
             ker((x[0] - 2 + U) / h) * ker((x[1] - 2 + V) / h);
    }

    return acc / (sample_size() * h * h);
  }

  virtual num_t density(const vec_t& x) const {
    gaussian_ker ker(1);
    return mirror_density(x, ker);
  }

  result<void*> export_density(std::string path_to_data) {
    std::ofstream stream;
    stream.open(path_to_data.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      return result<void*>::error(strerror(errno), error_t::io_error);
    }

    vec_t samples;
    for (size_t i = 0; i < _grid.size(); ++i) {
      for (size_t k = 0; k < _dim; ++k) {
        stream << _grid[i][k] << ' ';
      }
      stream << density(_grid[i]) << '\n';
    }
    stream.close();
    return result<void*>::ok(NULL);
  }

  virtual size_t dim() const { return _dim; }

  size_t sample_size() const { return _mv_dist->sample_size(); }

private:
  size_t _dim;
  const emv_distribution_t* _mv_dist;
};
}

#endif
