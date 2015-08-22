//
//  distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__distribution__
#define __corrstat__distribution__

#include <cerrno>
#include <ctime>
#include <cstring>
#include <fstream>

#include "distribution_i.h"

/**
 *  This module contains definition of the distribution class.
 */

namespace cst {

/**
 *  distribution class provides a set of common operations over pure univariate
 *  distribution.
 */
class distribution_t : public distribution_i {
public:
  virtual num_t prob(num_t x) const { return 0; }
  virtual num_t density(num_t x) const = 0;
  virtual num_t cdf(num_t x) const = 0;
  virtual num_t inv_cdf(num_t p) const = 0;

  virtual num_t mean() const = 0;
  virtual num_t variance() const = 0;

  virtual num_t supp_lower_bound() const = 0;
  virtual num_t supp_upper_bound() const = 0;

  virtual bool is_supp_lower_bound_inclusive() const = 0;
  virtual bool is_supp_upper_bound_inclusive() const = 0;

  virtual num_t sample() const = 0;

  /**
   *  Export current grid values and corresponding cdf values to a file as two
   *  columns.
   *
   *  @param path_to_data Path to write data to.
   *
   *  @return OK result for success, error otherwise.
   */
  result<void*> export_cdf(std::string path_to_data) {
    std::ofstream stream;
    stream.open(path_to_data.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<void*>::error(err_text, error_t::io_error);
    }

    vec_t sample;
    for (size_t i = 0; i < _grid.size(); ++i) {
      stream << _grid[i][0] << ' ' << cdf(_grid[i][0]) << '\n';
    }
    stream.close();
    return result<void*>::ok(NULL);
  }

  /**
   *  Export current grid values and corresponding density values to a file as
   *  two columns.
   *
   *  @param path_to_data Path to write data to.
   *
   *  @return OK result for success, error otherwise.
   */
  result<void*> export_density(std::string path_to_data) {
    std::ofstream stream;
    stream.open(path_to_data.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<void*>::error(err_text, error_t::io_error);
    }

    vec_t sample;
    for (size_t i = 0; i < _grid.size(); ++i) {
      stream << _grid[i][0] << ' ' << density(_grid[i][0]) << '\n';
    }
    stream.close();
    return result<void*>::ok(NULL);
  }

  /**
   *  Get multiple samples at once.
   *
   *  @param size Number of needed samples.
   *
   *  @return Vector of samples.
   */
  virtual vec_t samples(size_t size) const {
    vec_t samples(size);

    for (size_t i = 0; i < size; ++i) {
      samples[i] = sample();
    }
    return samples;
  }
};
}

#endif /* defined(__corrstat__distribution__) */
