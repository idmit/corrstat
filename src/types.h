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
#include <vector>

#include "result.h"

namespace cst {
typedef double num_t;
typedef std::vector<num_t> vec_t;

typedef num_t (*fn)(num_t);
typedef num_t (*mv_fn)(vec_t);

const num_t PI = 3.14159265359;

inline num_t cross(const vec_t &x, const vec_t &y) {
  assert(x.size() == y.size());
  num_t c = 0;
  for (size_t i = 0; i < x.size(); ++i) {
    c += x[i] * y[i];
  }
  return c;
}

inline bool less(const vec_t &lhs, const vec_t &rhs) {
  assert(lhs.size() == rhs.size());
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] > rhs[i]) {
      return false;
    }
  }
  return true;
}
}

#endif
