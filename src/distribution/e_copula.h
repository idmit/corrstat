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
#include "mv_distribution.h"
#include "normal_distribution.h"

namespace cst {

class e_copula_t : public copula_t {
public:
  e_copula_t(const mv_distribution_t* mv_dist)
      : copula_t(mv_dist->dim()), _mv_dist(mv_dist) {}

  virtual num_t call(const vec_t& x) const {
    // TODO: Implement if needed.
    assert(false);
    return 0;
  }

  virtual num_t operator()(const vec_t& x) const { return call(x); }

  virtual num_t mirror_density(const vec_t& x, gaussian_ker ker,
                               num_t h = 0.05) const {
    num_t acc = 0;

    for (size_t i = 0; i < dist_grid_size(); ++i) {
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

    return acc / (dist_grid_size() * h * h);
  }

  virtual num_t transformed_density(const vec_t& x, mv_gaussian_ker ker,
                                    num_t h = 0.05) const {
    num_t acc = 0;
    normal_distribution_t gauss(0, 1);
    num_t gu = gauss.inv_cdf(x[0]), gv = gauss.inv_cdf(x[1]);

    for (size_t i = 0; i < dist_grid_size(); ++i) {

      num_t U = _mv_dist->margin_cdf_on_grid(0, i);
      num_t V = _mv_dist->margin_cdf_on_grid(1, i);

      num_t gU = gauss.inv_cdf(U), gV = gauss.inv_cdf(V);

      acc += ker((gu - gU) / h, (gv - gV) / h);
    }

    return acc /
           (dist_grid_size() * h * h * gauss.density(gu) * gauss.density(gv));
  }

  virtual num_t density(const vec_t& x) const {
    gaussian_ker ker;
    return mirror_density(x, ker);
  }

  size_t dist_grid_size() const { return _mv_dist->grid().size(); }

  virtual result<void*> export_density(std::string path_to_data) {
    _mv_dist->calc_margin_cdfs_on_grid();
    return copula_t::export_density(path_to_data);
  }

private:
  const mv_distribution_t* _mv_dist;
};
}

#endif
