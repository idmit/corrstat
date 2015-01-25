//
//  estimator.h
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__estimator__
#define __corrstat__estimator__

#include "data.h"
#include "estkind.h"

namespace cst {
class estimator {
public:
  estimator();
  double exec(data &samples);
  bool load(std::string dir_path, est_kind::t kind);
  ~estimator();

private:
  double (*_extern_exec)(data samples);
  void *_lib;
};
}

#endif /* defined(__corrstat__estimator__) */
