//
//  emv_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 11/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_emv_distribution_h
#define corrstat_emv_distribution_h

#include "mv_distribution_i.h"

namespace cst {

class emv_distribution_t : public mv_distribution_i {
public:
  emv_distribution_t(const std::vector<vec_t> &mv_samples)
      : _dim(mv_samples.size()), _mv_samples(mv_samples) {}

  virtual num_t density(vec_t x) { return 0; }

  virtual num_t cdf(vec_t x) {
    num_t num = 0;
    for (size_t i = 0; i < _mv_samples[0].size(); ++i) {
      bool ok = true;
      for (size_t k = 0; k < _mv_samples.size(); ++k) {
        if (_mv_samples[k][i] > x[k]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        num += 1;
      }
    }
    return num / _mv_samples[0].size();
  }

  virtual size_t dim() { return _dim; }
  virtual vec_t sample() {
    vec_t s(1);
    s[0] = 0;
    return s;
  }

protected:
  size_t _dim;
  std::vector<vec_t> _mv_samples;
};
}

#endif
