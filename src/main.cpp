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
  //  cst::gaussian_copula_t g_cop(2, 0.4);
  //    dist.borrow()->set_sample_as_grid();

  //  g_cop.set_grid(cst::vec_t(2, 0.01), cst::vec_t(2, 0.99), 30);

  //  result<void *> res = g_cop.export_density(argv[2]);
  //    result<void *> res = (*dist).export_cdf(argv[2]);
  //  if (!res.is_ok()) {
  //    printf("%s\n", res.err().what().c_str());
  //  } else {
  //    printf("%s\n", "Finished.");
  //  }
  //  } else {
  //    printf("%s\n", dist.err().what().c_str());
  //  }

  result<hope_t> hope = hope_t::make("", "", "");

  if (!hope.is_ok()) {
    printf("%s\n", hope.err().what().c_str());
  }

  size_t params[] = { 0, 1, 4, 1 };
  hope.borrow()->export_estimations(argv[1], argv[2], argv[3], params, argv[4]);

  return 0;
}
