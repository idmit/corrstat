//
//  copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_copula_h
#define corrstat_copula_h

#include "copula_i.h"

namespace cst {

class copula_t : public copula_i {
public:
  copula_t(size_t dim) : _dim(dim) {}

  virtual num_t call(vec_t x) = 0;
  virtual num_t operator()(vec_t x) { return call(x); }
  virtual size_t dim() const { return _dim; }

protected:
  void check(const vec_t& x) { assert(x.size() == _dim); }
  size_t _dim;
};
}

#endif
