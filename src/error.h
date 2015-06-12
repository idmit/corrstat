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

class error_t {
public:
  enum type { runtime_error, io_error };

  error_t(const std::string &what, error_t::type type = runtime_error);
  const std::string &what() const;
  error_t::type how() const;

private:
  std::string _what;
  error_t::type _type;
};

#endif /* defined(__corrstat__error__) */
