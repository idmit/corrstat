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

  virtual num_t call(const vec_t& x) const = 0;
  virtual num_t operator()(const vec_t& x) const { return call(x); }
  virtual num_t density(const vec_t& x) const = 0;
  virtual size_t dim() const { return _dim; }

protected:
  size_t _dim;
};
}

#endif
