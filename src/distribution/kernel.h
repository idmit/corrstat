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

class mv_kernel {
public:
  virtual num_t operator()(vec_t x) const = 0;
};

class gaussian_ker : public kernel {
public:
  gaussian_ker(num_t sd = 1) : _sd(sd) {}

  virtual num_t operator()(num_t x) const {
    return std::exp(-(x * x) / (2 * _sd * _sd)) / (_sd * std::sqrt(2 * PI));
  }

private:
  num_t _sd;
};

class mv_gaussian_ker : public mv_kernel {
public:
  num_t operator()(num_t x, num_t y) const {
    return std::exp(-(x * x + y * y) / 2) / (2 * PI);
  }

  virtual num_t operator()(vec_t x) const {
    num_t acc = 0;
    for (size_t i = 0; i < x.size(); ++i) {
      acc += x[i] * x[i];
    }
    return std::exp(-acc / 2) / std::sqrt(std::pow(2 * PI, x.size()));
  }
};
}

#endif
