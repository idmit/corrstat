//
//  distribution.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 07/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__distribution__
#define __corrstat__distribution__

#include "distribution_i.h"

namespace cst {

class distribution_t : public distribution_i {
  virtual num_t prob(num_t x) const { return 0; }
};
}

#endif /* defined(__corrstat__distribution__) */
