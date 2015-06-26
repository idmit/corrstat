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
  static result<void *> read_model_vals(const std::string &model_vals_filename,
                                        std::vector<vec_t> &model_vals);

  static result<void *> read_bounds(const std::string &bounds_filename,
                                    std::vector<vec_t> &bounds);

  static result<void *> read_hit_vals(const std::string &hit_numbers_filename,
                                      std::vector<vec_t> &hit_numbers);
  static result<void *> read_unique_model_vals(
      const std::string &base_path_to_read,
      std::vector<vec_t> &unique_model_vals);

  result<void *> export_middles(const std::string &bounds_filename,
                                const std::string &path_to_write) const;
  result<void *> export_unique_model_vals(
      const std::string &model_vals_filename,
      const std::string &base_path_to_write) const;

  result<void *> export_shifts(const std::string &model_vals_filename,
                               const std::string &path_to_middles,
                               const std::string &path_to_write) const;
  result<void *> export_marginals(const std::string &model_vals_filename,
                                  const std::string &path_to_shifts,
                                  const std::string &base_path_to_read,
                                  const std::string &path_to_write) const;
  result<void *> export_hit_marginals(const std::string &model_vals_filename,
                                      const std::string &path_to_hit_numbers,
                                      const std::string &base_path_to_read,
                                      const std::string &path_to_write) const;
  result<void *> export_joint(const std::string &model_vals_filename,
                              const std::string &path_to_shifts,
                              const std::string &base_path_to_read,
                              const size_t params_and_vals[4],
                              const std::string &path_to_write) const;
  result<void *> export_hit_joint(const std::string &model_vals_filename,
                                  const std::string &path_to_hits,
                                  const std::string &base_path_to_read,
                                  const std::string &path_to_write) const;
  result<void *> export_estimations(const std::string &path_to_margins,
                                    const std::string &path_to_joints,
                                    const std::string &base_path_to_read,
                                    const size_t params_and_vals[4],
                                    const std::string &path_to_write) const;

private:
  static void gnuplot(const std::string &filename, const std::string &plotname,
                      const std::string &scriptname, size_t x, size_t y,
                      num_t xval, num_t yval);
  static result<void *> load_csv(const std::string &filename,
                                 std::vector<vec_t> &values, size_t row_size,
                                 bool dynamic = false);
  static result<void *> save_csv(const std::string &filename,
                                 const std::vector<vec_t> &values,
                                 size_t row_size);
};
}

#endif /* defined(__corrstat__hope__) */
