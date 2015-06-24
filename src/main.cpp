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

#include "distribution/clayton_copula.h"
#include "distribution/gumbel_copula.h"
#include "distribution/frank_copula.h"
#include "distribution/uniform_distribution.h"
#include "distribution/normal_distribution.h"
#include "distribution/e_copula.h"
#include "distribution/e_distribution.h"
#include "distribution/cp_distribution.h"
#include "distribution/emv_distribution.h"

#include "hope.h"
#include "types.h"

using namespace cst;

int main(int argc, char **argv) {
  result<hope_t> hope = cst::hope_t::make(argv[1], "", "");

  if (hope) {
    result<void *> wrote =
        hope.borrow()->export_marginals(argv[2], argv[3], argv[4]);
  } else {
    printf("%s\n", hope.err().what().c_str());
  }

  return 0;
}
