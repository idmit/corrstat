//
//  estimator.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <dlfcn.h>
#include "estimator.h"

namespace cst {
estimator::estimator() : _extern_exec(0), _lib(0) {}

double estimator::exec(data samples) { return _extern_exec(samples); }

bool estimator::load(est_kind::t kind) {
  if (_lib != nullptr) {
    dlclose(_lib);
  }
  _lib = dlopen("/Users/ivandmi/Documents/dev/corrstat/corrstat/src/estimators/"
                "estkendall.so",
                RTLD_NOW);

  char *error = dlerror();
  if (error) {
    printf("failed to open kendall.so: %s \n", error);
    return false;
  };

  //  Hack, C++ doesn't support this.
  ul hack = (ul)dlsym(_lib, "exec");
  _extern_exec = (double (*)(data samples))hack;
  error = dlerror();

  if (error) {
    printf("failed to find symboll: %s \n", error);
    return false;
  }

  return true;
}

estimator::~estimator() {
  if (_lib != 0) {
    dlclose(_lib);
  }
}
}
