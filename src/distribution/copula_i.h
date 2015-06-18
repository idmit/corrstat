//
//  copula_i.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__copula_i__
#define __corrstat__copula_i__

#include "../types.h"
#include "mv_grid.h"

namespace cst {

class copula_i : public mv_grid_i {
public:
  virtual num_t operator()(const vec_t& x) const = 0;
  virtual num_t density(const vec_t& x) const = 0;
  virtual size_t dim() const = 0;

  virtual ~copula_i() {}
};
}

#endif /* defined(__corrstat__copula_i__) */
