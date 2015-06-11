//
//  result.h
//  corrstat
//
//  Created by Ivan Dmitrievsky on 09/06/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__result__
#define __corrstat__result__

#include "error.h"

template <class T> class result {
public:
  static result<T> ok(const T &value);
  static result<T> error(const std::string &what, error_t::type type);
  static result<T> error(const error_t &error);

  bool is_ok() const;
  operator bool() const;

  T get();
  T operator*();
  error_t err();

private:
  result(const T &value);
  result(const std::string &what, error_t::type type);
  bool _ok;
  T _value;
  error_t _error;
};

template <class T>
result<T>::result(const T &value)
    : _ok(true), _value(value), _error("") {}

template <class T>
result<T>::result(const std::string &what, error_t::type type)
    : _ok(false), _error(what, type) {}

template <class T> result<T> result<T>::ok(const T &value) {
  return result(value);
}

template <class T>
result<T> result<T>::error(const std::string &what, error_t::type type) {
  return result(what, type);
}

template <class T> result<T> result<T>::error(const error_t &error) {
  return result(error.what(), error.how());
}

template <class T> bool result<T>::is_ok() const { return _ok; }
template <class T> result<T>::operator bool() const { return _ok; }

template <class T> T result<T>::get() { return _value; }
template <class T> T result<T>::operator*() { return _value; }

template <class T> error_t result<T>::err() { return _error; }

#endif /* defined(__corrstat__result__) */
