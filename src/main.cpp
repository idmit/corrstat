//
//  main.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 10/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "hope.h"
#include "types.h"

using namespace cst;

int main(int argc, char **argv) {
  hope_t hope;

  result<void *> res =
      hope.export_hit_joint(argv[1], argv[2], argv[3], argv[4]);

  if (!res.is_ok()) {
    printf("%s\n", res.err().what().c_str());
  } else {
    printf("%s\n", "Finished.");
  }

  return 0;
}
