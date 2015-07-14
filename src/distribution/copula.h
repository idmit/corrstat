//
//  copula.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 10/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef corrstat_copula_h
#define corrstat_copula_h

#include <cerrno>
#include <cstring>
#include <fstream>

#include "copula_i.h"

namespace cst {

class copula_t : public copula_i {
public:
  copula_t(size_t dim) : _dim(dim) {}

  virtual num_t call(const vec_t& x) const = 0;
  virtual num_t operator()(const vec_t& x) const { return call(x); }
  virtual num_t density(const vec_t& x) const = 0;
  virtual size_t dim() const { return _dim; }

  virtual result<void*> export_density(std::string path_to_data) {
    std::ofstream stream;
    stream.open(path_to_data.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<void*>::error(err_text, error_t::io_error);
    }

    vec_t samples;
    for (size_t i = 0; i < _grid.size(); ++i) {
      for (size_t k = 0; k < _dim; ++k) {
        stream << _grid[i][k] << ' ';
      }
      stream << density(_grid[i]) << '\n';
    }
    stream.close();
    return result<void*>::ok(NULL);
  }

protected:
  size_t _dim;
};
}

#endif
