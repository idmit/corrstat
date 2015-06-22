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

int main(int argc, char **argv) {
  result<cst::emv_distribution_t> dist = cst::emv_distribution_t::read(argv[1]);
  if (dist) {
    cst::e_copula_t e_cop1(dist.borrow(), true);
    cst::e_copula_t e_cop2(dist.borrow(), false);

    dist.borrow()->set_sample_as_grid();

    e_cop1.set_grid(cst::vec_t(2, 0.05), cst::vec_t(2, 0.95), 30);
    e_cop2.set_grid(cst::vec_t(2, 0.05), cst::vec_t(2, 0.95), 30);

    result<void *> res = e_cop1.export_density(argv[2]);

    //    result<void *> res = (*dist).export_cdf(argv[2]);
    if (!res.is_ok()) {
      printf("%s\n", res.err().what().c_str());
    } else {
      printf("%s\n", "Finished.");
    }

    res = e_cop2.export_density(argv[3]);
    if (!res.is_ok()) {
      printf("%s\n", res.err().what().c_str());
    } else {
      printf("%s\n", "Finished.");
    }

  } else {
    printf("%s\n", dist.err().what().c_str());
  }

  return 0;
}
