//
//  error.cpp
//  corrstat
//
//  Created by Ivan Dmitrievsky on 09/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#include "error.h"

error_t::error_t(const std::string &what, error_t::type type)
    : _what(what), _type(type) {}

const std::string &error_t::what() const { return _what; }

error_t::type error_t::how() const { return _type; }
