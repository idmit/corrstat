//
//  estkind.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "estkind.h"

namespace cst {

const std::string est_kind::_names[] = {
#define X(a, b) #b
#include "estkinds.def"
#undef X
    , ""};

std::string est_kind::str(cst::est_kind::t kind) {
  return cst::est_kind::_names[(int)kind];
}

est_kind::t est_kind::enm(const char *cstring) {
  unsigned i = 0;
  std::string string(cstring);
  while (_names[i] != "") {
    if (string == _names[i]) {
      return (est_kind::t)i;
    }
    i++;
  }
  return (est_kind::t)0;
}
}
