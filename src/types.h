//
//  types.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_types_h
#define corrstat_types_h

#include <cassert>
#include <sstream>
#include <vector>

#include "result.h"

/**
 *  This module contains various type definitions, constants and utility
 *  functions.
 */

namespace cst {

/**
 *  Floating point type for all calculations.
 */
typedef double num_t;

/**
 *  Vector of chosen floating point type.
 */
typedef std::vector<num_t> vec_t;

/**
 *  Regular mathematical function type.
 */
typedef num_t (*fn)(num_t);

/**
 *  Multidimensional mathematical function type.
 */
typedef num_t (*mv_fn)(vec_t);

const num_t PI = 3.14159265359;

/**
 *  Utility function to convert numeric types to string.
 *
 *  @param num Number to convert.
 *
 *  @return String representing given number.
 */
template <typename T> std::string to_string(T num) {
  std::ostringstream ss;
  ss << num;
  return ss.str();
}
}

#endif
