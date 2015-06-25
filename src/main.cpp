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
#include "distribution/gaussian_copula.h"
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
  //  result<cst::emv_distribution_t> dist =
  //  cst::emv_distribution_t::read(argv[1]);
  //  if (dist) {
  //    cst::e_copula_t e_cop(dist.borrow());
  cst::gaussian_copula_t g_cop(2, 0.41);
  //    dist.borrow()->set_sample_as_grid();

  g_cop.set_grid(cst::vec_t(2, 0.01), cst::vec_t(2, 0.99), 30);

  result<void *> res = g_cop.export_density(argv[1]);
  //      result<void *> res = (*dist).export_cdf(argv[2]);
  if (!res.is_ok()) {
    printf("%s\n", res.err().what().c_str());
  } else {
    printf("%s\n", "Finished.");
  }
  //  } else {
  //    printf("%s\n", dist.err().what().c_str());
  //  }

  return 0;
}
