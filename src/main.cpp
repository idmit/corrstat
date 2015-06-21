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
#include "distribution/e_copula.h"
#include "distribution/e_distribution.h"
#include "distribution/cp_distribution.h"

int main(int argc, char **argv) {
  cst::frank_copula_t cop(2, 1000);
  cst::uniform_distribution_t uni(0, 1);

  cst::e_distribution_t x(uni.samples(500)), y(uni.samples(500));

  std::vector<const cst::distribution_t *> margins;
  margins.push_back(&x);
  margins.push_back(&y);

  cst::cp_distribution_t dist(&cop, margins);

  dist.set_grid(cst::vec_t(2, -2), cst::vec_t(2, 3), 30);

  cst::e_copula_t e_cop(&dist);
  e_cop.set_grid(cst::vec_t(2, -2), cst::vec_t(2, 3), 30);

  result<void *> res = e_cop.export_density(argv[1]);
  if (!res.is_ok()) {
    printf("%s\n", res.err().what().c_str());
  } else {
    printf("%s\n", "Finished.");
  }

  return 0;
}
