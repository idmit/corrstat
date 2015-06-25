//
//  distribution.cpp
//  corrstat
//
//  Created by Ivan Dmitrievsky on 25/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "distribution.h"

namespace cst {
boost::random::mt19937 distribution_t::_eng =
    boost::random::mt19937((unsigned)std::time(NULL));
}
