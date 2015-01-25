//
//  spn.h
//  corrstat
//
//  Created by Иван Дмитриевский on 23/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__spn__
#define __corrstat__spn__

#include "spn/ctx.h"
#include <string>
#include <vector>

namespace cst {

typedef int (*spn_ext_fn)(SpnValue *, int, SpnValue *, void *);

class spn {
public:
  spn();
  int exec(std::string script_path);
  ~spn();

private:
  static void wrap_fn(const char *name, spn_ext_fn fn);
  void load_fns();

private:
  SpnContext _ctx;
  static std::vector<SpnExtFunc> _ext_fns;
};
}

#endif /* defined(__corrstat__spn__) */
