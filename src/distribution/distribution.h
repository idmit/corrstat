//
//  distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__distribution__
#define __corrstat__distribution__

#include "distribution_i.h"

namespace cst {

class distribution_t : public distribution_i {
public:
  virtual num_t prob(num_t x) const { return 0; }
  virtual num_t density(num_t x) const = 0;
  virtual num_t cdf(num_t x) const = 0;
  virtual num_t inv_cdf(num_t p) const = 0;

  virtual num_t mean() const = 0;
  virtual num_t variance() const = 0;

  virtual num_t supp_lower_bound() const = 0;
  virtual num_t supp_upper_bound() const = 0;

  virtual bool is_supp_lower_bound_inclusive() const = 0;
  virtual bool is_supp_upper_bound_inclusive() const = 0;

  virtual num_t sample() const = 0;
};
}

#endif /* defined(__corrstat__distribution__) */
