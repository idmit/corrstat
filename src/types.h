//
//  types.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_types_h
#define corrstat_types_h

#include <cassert>

#include "result.h"

namespace cst {
typedef double num_t;
typedef std::vector<num_t> vec_t;

typedef num_t (*fn)(num_t);
typedef num_t (*mv_fn)(vec_t);

num_t cross(vec_t x, vec_t y) {
  assert(x.size() == y.size());
  num_t c = 0;
  for (size_t i = 0; i < x.size(); ++i) {
    c += x[i] * y[i];
  }
  return c;
}
}

#endif
