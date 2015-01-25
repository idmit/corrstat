//
//  estkind.cpp
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "estkind.h"

const std::string cst::est_kind::_names[] = {
#define X(a, b) #b
#include "estkinds.def"
#undef X
};
