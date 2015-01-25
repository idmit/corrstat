//
//  estkind.h
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_estkind_h
#define corrstat_estkind_h

#include <string>
#include <vector>

namespace cst {
class est_kind {
public:
  enum t {
#define X(a, b) a
#include "estkinds.def"
#undef X
  };

  static std::string str(est_kind::t kind);
  static t enm(const char *cstring);

private:
  static const std::string _names[];
};
}

#endif
