//
//  data.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "data.h"
#include <cassert>

namespace cst {
data::data(ul dim, ul size)
    : eq(_eq), lt(_lt), gt(_gt), cmp(_cmp), _dim(dim), _raw(size) {}

bool data::_eq(point l, point r) {
  assert(l.size() == r.size());
  ul dim = l.size();
  for (ul i = 0; i < dim; ++i) {
    if (l[i] != r[i]) {
      return false;
    }
  }
  return true;
}

bool data::_lt(point l, point r) {
  assert(l.size() == r.size());
  ul dim = l.size();
  ul i = 0;
  for (i = 0; i < dim; ++i) {
    if (l[i] != r[i]) {
      break;
    }
  }

  return i < dim && l[i] < r[i];
}

bool data::_gt(point l, point r) {
  assert(l.size() == r.size());
  ul dim = l.size();
  ul i = 0;
  for (i = 0; i < dim; ++i) {
    if (l[i] != r[i]) {
      break;
    }
  }

  return i < dim && l[i] > r[i];
}

short data::_cmp(point l, point r) {
  if (_eq(l, r)) {
    return 0;
  } else if (_lt(l, r)) {
    return -1;
  } else {
    return 1;
  }
}
}
