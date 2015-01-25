//
//  session.h
//  corrstat
//
//  Created by Иван Дмитриевский on 25/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__session__
#define __corrstat__session__

#include "task.h"
#include <string>
#include <list>

namespace cst {

class session {
public:
  static task *task_from_file(const char *path);
  static task *task_from_db(std::string db_name, std::string query);

private:
  session();

private:
  static std::list<task> _storage;
};
}

#endif /* defined(__corrstat__session__) */
