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

  virtual num_t cdf(const vec_t& x) {
    vec_t cp(_dim);
    for (size_t i = 0; i < _dim; ++i) {
      cp[i] = _margins[i]->cdf(x[i]);
    }

    return _copula->call(cp);
  }

private:
  const copula_t* _copula;
  std::vector<const distribution_t*> _margins;
};
}

#endif
