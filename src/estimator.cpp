//
//  estimator.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <dlfcn.h>
#include "estimator.h"

// double cst::estimator::exec(cst::data samples) { return
// _extern_exec(samples); }

bool cst::estimator::load(cst::est_kind::t kind) {
  if (_lib != nullptr) {
    dlclose(_lib);
  }
  _lib = dlopen("/Users/ivandmi/Downloads/kendall.so", RTLD_NOW);

  char *error = dlerror();
  if (error) {
    //    printf("failed to open kendall.so: %s \n", error);
    return false;
  };

  //  _extern_exec = (double (*)(data samples))dlsym(_lib, "exec");
  error = dlerror();

  if (error) {
    //    printf("failed to find symboll: %s \n", error);
    return false;
  }

  return true;
}

cst::estimator::~estimator() {
  if (_lib != nullptr) {
    dlclose(_lib);
  }
}
