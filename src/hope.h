//
//  hope.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 23/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__hope__
#define __corrstat__hope__

#include "./types.h"

namespace cst {
class hope_t {
public:
  hope_t();
  hope_t(const std::vector<vec_t> &model_vals, const std::vector<vec_t> &bounds,
         const std::vector<vec_t> &hit_numbers);
  static result<hope_t> make(const std::string &model_vals_filename,
                             const std::string &bounds_filename,
                             const std::string &hit_numbers_filename);

  result<void *> export_middles(const std::string &path_to_data) const;
  result<void *> export_unique_model_vals(
      const std::string &path_to_write) const;
  result<void *> export_shifts(const std::string &path_to_middles,
                               const std::string &path_to_write) const;
  result<void *> export_marginals(const std::string &path_to_shifts,
                                  const std::string &base_path_to_read,
                                  const std::string &path_to_write) const;

private:
  static result<void *> load_csv(const std::string &filename,
                                 std::vector<vec_t> &values, size_t row_size,
                                 bool dynamic = false);
  static result<void *> save_csv(const std::string &filename,
                                 const std::vector<vec_t> &values,
                                 size_t row_size);

  std::vector<vec_t> _model_vals;
  std::vector<vec_t> _bounds;
  std::vector<vec_t> _hit_numbers;
};
}

#endif /* defined(__corrstat__hope__) */
