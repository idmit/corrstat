//
//  mv_distribution_i.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__mv_distribution_i__
#define __corrstat__mv_distribution_i__

#include "../types.h"

/**
 *  This module contains definition of the mv_distribution interface.
 */

namespace cst {

/**
 *  mv_distribution interface provides a set of common operations over
 *  multivariate distribution.
 */
class mv_distribution_i : public mv_grid_i {
public:
  /**
   *  Get a value of probability density function at given argument x.
   *
   *  @param x Argument of interest.
   *
   *  @return Value of probability density function at given argument.
   */
  virtual num_t density(const vec_t& x) const = 0;

  /**
   *  Get a value of cumulative distribution function at given argument x.
   *
   *  @param x Argument of interest.
   *
   *  @return Value of cumulative probability density function at given
   *  argument.
   */
  virtual num_t cdf(const vec_t& x) const = 0;

  /**
   *  Get a value of marginal cumulative distribution function at given
   *  argument x.
   *
   *  @param coordinate Coordinate corresponding to margin.
   *  @param x          Argument of interest.
   *
   *  @return Value of marginal cumulative distribution function at given
   *  argument.
   */
  virtual num_t margin_cdf(size_t coordinate, num_t x) const = 0;

  /**
   *  Get the distribution's dimensions.
   *
   *  @return Number of dimensions.
   */
  virtual size_t dim() const = 0;

  /**
   *  Get a sample.
   *
   *  @return Sample value.
   */
  virtual vec_t sample() const = 0;

  /**
   *  Virtual destructor for building hierarchy.
   */
  virtual ~mv_distribution_i(){};
};
}

#endif /* defined(__corrstat__mv_distribution_i__) */
