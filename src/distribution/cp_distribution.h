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

#include "shared_ptr.hpp"

namespace cst {

class cp_distribution_t : public mv_distribution_t {
public:
  cp_distribution_t(copula_t* copula,
                    const std::vector<distribution_t*>& margins)
      : mv_distribution_t(copula->dim()), _copula(copula) {
    assert(copula->dim() == margins.size());
    _margins.reserve(margins.size());
    for (size_t i = 0; i < margins.size(); ++i) {
      _margins.push_back(shared_ptr<distribution_t>(margins[i]));
    }
  }

  virtual num_t cdf(const vec_t& x) {
    vec_t cp(_dim);
    for (size_t i = 0; i < _dim; ++i) {
      cp[i] = _margins[i]->cdf(x[i]);
    }

    return _copula->call(cp);
  }

private:
  shared_ptr<copula_t> _copula;
  std::vector<shared_ptr<distribution_t> > _margins;
};
}

#endif
