//
//  task.h
//  corrstat
//
//  Created by Иван Дмитриевский on 11/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__task__
#define __corrstat__task__

#include "data.h"
#include "estkind.h"

namespace cst {

class task {
public:
  task(data data);
  double exec(est_kind::t kind);

private:
  data _data;
};
}

#endif /* defined(__corrstat__task__) */
