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
std::string estimator::_base_dir = std::string();
std::string estimator::_rel_path = std::string();

estimator::estimator() : _extern_exec(0), _lib(0) {}

double estimator::exec(data &samples) { return _extern_exec(samples); }

bool estimator::load(est_kind::t kind) {
  if (_lib != 0) {
    dlclose(_lib);
  }

  std::string lib_name = "est" + est_kind::str(kind) + ".so";
  // dir_path += dir_path.back() == '/' ? "" : "/";
  std::string dl_path = _base_dir + lib_name;

  _lib = dlopen(dl_path.c_str(), RTLD_NOW);

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

void estimator::set_base_dir(std::string script_dir) {
  _base_dir = script_dir + _rel_path;
}

void estimator::set_rel_path(std::string rel_path) { _rel_path = rel_path; }

estimator::~estimator() {
  if (_lib != 0) {
    dlclose(_lib);
  }
}
}
