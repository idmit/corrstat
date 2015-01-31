//
//  main.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 10/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <iostream>
#include "estimator.h"
#include "spn.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }
  std::string default_rel = "estimators";
  default_rel.push_back(cst::path_sep);
  cst::estimator::set_rel_path(argc > 2 ? std::string(argv[3]) : default_rel);
  cst::spn spn;
  spn.exec(argv[1]);
}
