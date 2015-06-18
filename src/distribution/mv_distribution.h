//
//  mv_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__mv_distribution__
#define __corrstat__mv_distribution__

#include "mv_distribution_i.h"

namespace cst {

class mv_distribution_t : public mv_distribution_i {
public:
  mv_distribution_t(size_t dim) : _dim(dim) {}
  virtual num_t density(const vec_t& x) const = 0;
  virtual num_t cdf(const vec_t& x) const = 0;
  virtual size_t dim() const { return _dim; }
  virtual vec_t sample() const = 0;

protected:
  size_t _dim;
};
}

#endif /* defined(__corrstat__mv_distribution__) */
