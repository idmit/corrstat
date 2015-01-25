//
//  estimator.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <dlfcn.h>
#include <iostream>
#include "estimator.h"

namespace cst {
estimator::estimator() : _extern_exec(0), _lib(0) {}

double estimator::exec(data samples) { return _extern_exec(samples); }

bool estimator::load(std::string dir_path, est_kind::t kind) {
  if (_lib != 0) {
    dlclose(_lib);
  }

  std::string lib_name = "est" + est_kind::s(kind) + ".so";
  dir_path += dir_path.back() == '/' ? "" : "/";
  dir_path += lib_name;

  _lib = dlopen(dir_path.c_str(), RTLD_NOW);

  char *error = dlerror();
  if (error) {
    std::cout << "Failed to open " + lib_name + ": " << error << ".\n";
    return false;
  };

  ul hack = (ul)dlsym(_lib, "exec");
  _extern_exec = (double (*)(data samples))hack;
  error = dlerror();

  if (error) {
    std::cout << "Failed to find symbol in " + lib_name + ": " << error
              << ".\n";
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
