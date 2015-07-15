//
//  error.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 09/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__error__
#define __corrstat__error__

#include <string>

/**
 *  This module contains definition of the error type.
 */

/**
 *  Error class storing cause of an error and its type.
 */
class error_t {
public:
  /**
   Different types of representable errors.
   */
  enum type { runtime_error, io_error };

  /**
   *  Constructor accepting error cause and type, which defaults to runtime
   *  error.
   */
  error_t(const std::string &what, error_t::type type = runtime_error);

  /**
   *  Getter for the cause field.
   *
   *  @return Description of the error cause.
   */
  const std::string &what() const;

  /**
   *  Getter for the error type.
   *
   *  @return Type of the error.
   */
  error_t::type how() const;

private:
  std::string _what;
  error_t::type _type;
};

#endif /* defined(__corrstat__error__) */
