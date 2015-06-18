//
//  kernel.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 17/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_kernel_h
#define corrstat_kernel_h

#include "../types.h"

namespace cst {

class kernel {
public:
  virtual num_t operator()(num_t x) const = 0;
};

class gaussian_ker : public kernel {
public:
  gaussian_ker(num_t sd) : _sd(sd) {}

  virtual num_t operator()(num_t x) const {
    return std::exp(-(x * x) / (2 * _sd * _sd)) / (_sd * std::sqrt(2 * PI));
  }

private:
  num_t _sd;
};
}

#endif
