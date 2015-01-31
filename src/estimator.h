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
  bool load(est_kind::t kind);
  ~estimator();

  static void set_base_dir(std::string script_dir);
  static void set_rel_path(std::string rel_path);

private:
  static std::string _base_dir;
  static std::string _rel_path;
  double (*_extern_exec)(data samples);
  void *_lib;
};
}

#endif /* defined(__corrstat__estimator__) */
