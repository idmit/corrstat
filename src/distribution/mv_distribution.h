//
//  mv_distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__mv_distribution__
#define __corrstat__mv_distribution__

#include <cerrno>
#include <fstream>
#include <sstream>

#include "mv_distribution_i.h"

namespace cst {

class mv_distribution_t : public mv_distribution_i {
public:
  mv_distribution_t(size_t dim) : _dim(dim) {}
  virtual num_t density(const vec_t& x) const = 0;
  virtual num_t cdf(const vec_t& x) const = 0;
  virtual num_t margin_cdf(size_t coordinate, num_t x) const = 0;
  virtual size_t dim() const { return _dim; }
  virtual vec_t sample() const = 0;

  virtual void calc_margin_cdfs_on_grid() const {
    for (size_t i = 0; i < _grid.size(); ++i) {
      vec_t tmp(_dim);
      _margin_cdfs_on_grid.push_back(tmp);
      for (size_t coordinate = 0; coordinate < _dim; ++coordinate) {
        _margin_cdfs_on_grid[i][coordinate] =
            margin_cdf(coordinate, _grid[i][coordinate]);
      }
    }
  }

  virtual num_t margin_cdf_on_grid(size_t coordinate,
                                   size_t element_num) const {
    return _margin_cdfs_on_grid[element_num][coordinate];
  }

  virtual result<void*> export_cdf(std::string path_to_data) {
    std::ofstream stream;
    stream.open(path_to_data.c_str(), std::ofstream::trunc);

    if (stream.fail()) {
      std::string err_text = path_to_data + std::string(": ") + strerror(errno);
      return result<void*>::error(err_text, error_t::io_error);
    }

    vec_t sample;
    for (size_t i = 0; i < _grid.size(); ++i) {
      for (size_t k = 0; k < _dim; ++k) {
        stream << _grid[i][k] << ' ';
      }
      stream << cdf(_grid[i]) << '\n';
    }
    stream.close();
    return result<void*>::ok(NULL);
  }

protected:
  size_t _dim;
  mutable std::vector<vec_t> _margin_cdfs_on_grid;
};
}

#endif /* defined(__corrstat__mv_distribution__) */
