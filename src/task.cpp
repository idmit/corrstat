//
//  task.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 11/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "task.h"
#include "estimator.h"

namespace cst {
cst::task::task(cst::data data) : _data(data) {}

double cst::task::exec(cst::est_kind::t kind) {
  cst::estimator estimator;
  double estimate = 0;

  if (estimator.load(kind)) {
    estimate = estimator.exec(_data);
  }

  return estimate;
}
}
