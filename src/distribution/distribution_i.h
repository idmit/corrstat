//
//  distribution_i.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__distribution_i__
#define __corrstat__distribution_i__

#include "../types.h"

namespace cst {

class distribution_i {
public:
  virtual num_t prob(num_t x) = 0;
  virtual num_t density(num_t x) = 0;
  virtual num_t cdf(num_t x) = 0;

  virtual num_t mean() = 0;
  virtual num_t variance() = 0;

  virtual num_t supp_lower_bound() = 0;
  virtual num_t supp_upper_bound() = 0;

  virtual bool is_supp_lower_bound_inclusive() = 0;
  virtual bool is_supp_upper_bound_inclusive() = 0;

  virtual num_t sample() = 0;

  virtual ~distribution_i(){};
};
}

#endif /* defined(__corrstat__distribution_i__) */
