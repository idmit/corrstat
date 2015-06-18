//
//  mv_distribution_i.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__mv_distribution_i__
#define __corrstat__mv_distribution_i__

#include "../types.h"

namespace cst {

class mv_distribution_i : public mv_grid_i {
public:
  virtual num_t density(const vec_t& x) const = 0;
  virtual num_t cdf(const vec_t& x) const = 0;
  virtual num_t margin_cdf(size_t coordinate, num_t x) const = 0;
  virtual size_t dim() const = 0;
  virtual vec_t sample() const = 0;

  virtual ~mv_distribution_i(){};
};
}

#endif /* defined(__corrstat__mv_distribution_i__) */
