//
//  main.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 10/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include <iostream>
#include "spn.h"
#include "data.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }
  cst::spn spn;
  spn.exec(argv[1]);
}
