//
//  session.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 25/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "session.h"

namespace cst {
std::list<task> session::_storage = std::list<task>();

task *session::task_from_file(const char *path) {
  _storage.push_back(task(data(5)));
  printf("%s\n", path);
  return &_storage.back();
}
}
