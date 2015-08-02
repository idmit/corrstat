//
//  distribution_i.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__distribution_i__
#define __corrstat__distribution_i__

#include "mv_grid.h"
#include "../types.h"

/**
 *  This module contains definition of the distribution interface.
 */

namespace cst {

/**
 *  distribution interface provides a set of common operations over univariate
 *  distribution.
 */
class distribution_i : public mv_grid_i {
public:
  /**
   *  Get a probability of given value x.
   *
   *  @param x Value of interest.
   *
   *  @return Probability of such an outcome.
   */
  virtual num_t prob(num_t x) const = 0;

  /**
   *  Get a value of probability density function at given argument x.
   *
   *  @param x Argument of interest.
   *
   *  @return Value of probability density function at given argument.
   */
  virtual num_t density(num_t x) const = 0;

  /**
   *  Get a value of cumulative distribution function at given argument x.
   *
   *  @param x Argument of interest.
   *
   *  @return Value of cumulative distribution function at given argument.
   */
  virtual num_t cdf(num_t x) const = 0;

  /**
   *  Get a value of inverse cumulative distribution function at given argument
   *  x.
   *
   *  @param x Argument of interest.
   *
   *  @return Value of inverse cumulative distribution function at given
   *  argument.
   */
  virtual num_t inv_cdf(num_t p) const = 0;

  /**
   *  Get the mean.
   *
   *  @return Distribution's mean.
   */
  virtual num_t mean() const = 0;

  /**
   *  Get the variance.
   *
   *  @return Distribution's variance.
   */
  virtual num_t variance() const = 0;

  /**
   *  Get the lower bound of the distribution's support set.
   *
   *  @return Lower bound of the distribution's support set.
   */
  virtual num_t supp_lower_bound() const = 0;

  /**
   *  Get the upper bound of the distribution's support set.
   *
   *  @return Upper bound of the distribution's support set.
   */
  virtual num_t supp_upper_bound() const = 0;

  /**
   *  Determine if lower bound of the distribution's support is inclusive.
   *
   *  @return true for inclusive, false otherwise.
   */
  virtual bool is_supp_lower_bound_inclusive() const = 0;

  /**
   *  Determine if upper bound of the distribution's support is inclusive.
   *
   *  @return true for inclusive, false otherwise.
   */
  virtual bool is_supp_upper_bound_inclusive() const = 0;

  /**
   *  Get a sample.
   *
   *  @return Sample value.
   */
  virtual num_t sample() const = 0;

  /**
   *  Virtual destructor for building hierarchy.
   */
  virtual ~distribution_i(){};
};
}

#endif /* defined(__corrstat__distribution_i__) */
