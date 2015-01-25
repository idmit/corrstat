//
//  session.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 25/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "session.h"

#include <cstdlib>
#include <cassert>
#include <fstream>
#include <sstream>

static cst::point tokenize(std::string string) {
  cst::point p;
  std::istringstream iss(string);
  std::string token;
  while (getline(iss, token, ' ')) {
    p.push_back(atof(token.c_str()));
  }
  return p;
}

namespace cst {
std::list<task> session::_storage = std::list<task>();

task *session::task_from_file(const char *path) {
  std::ifstream in_file(path);
  std::string line;
  std::vector<point> tmp;
  ul dim = 0;

  std::getline(in_file, line);
  tmp.push_back(tokenize(line));
  dim = tmp.back().size();

  while (in_file && std::getline(in_file, line)) {
    tmp.push_back(tokenize(line));
    assert(dim == tmp.back().size());
  }

  data d(dim, tmp.size());

  for (ul i = 0; i < tmp.size(); ++i) {
    d[i] = tmp[i];
  }

  _storage.push_back(task(d));
  return &_storage.back();
}
}
