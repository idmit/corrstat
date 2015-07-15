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

/**
 *  This module contains definition of the result type, that is widely used in
 *  other modules.
 */

/**
 *  Result class storing some value of type T as a result of some operation
 *  returning T, if that operation succeded, or an error value, if it failed.
 *  Type T should contain a default constructor.
 */
template <class T> class result {
public:
  /**
   *  Constructor accepting value to represent success.
   *
   *  @param value Value of some operation.
   *
   *  @return Result containing given value.
   */
  static result<T> ok(const T &value);

  /**
   *  Constructor accepting error parameters to represent failure.
   *
   *  @param what Cause of error.
   *  @param type Type of error.
   *
   *  @return Result representing failure with given error.
   */
  static result<T> error(const std::string &what, error_t::type type);

  /**
   *  Constructor accepting already created error.
   *
   *  @param error Error that occured earlier.
   *
   *  @return Result representing failure with given error.
   */
  static result<T> error(const error_t &error);

  /**
   *  Getter for outcome represented by the result.
   *
   *  @return true for success, false for faulure.
   */
  bool is_ok() const;

  /**
   *  Implicit getter for outcome represented by the result.
   *
   *  @return true for success, false for faulure.
   */
  operator bool() const;

  /**
   *  Getter for the value, if result represents success. Don't call it
   *  otherwise.
   *
   *  @return Copy of the value corresponding to success.
   */
  T get();

  /**
   *  Implicit getter for the pointer to the value, if result represents
   *  success. Don't call it otherwise.
   *
   *  @return Pointer to the value corresponding to success.
   */
  T *operator->();

  /**
   *  Getter for the pointer to the value, if result represents success. Don't
   *  call it otherwise.
   *
   *  @return Pointer to the value corresponding to success.
   */
  const T *borrow();

  /**
   *  Imlicit getter for the value, if result represents success. Don't call it
   *  otherwise.
   *
   *  @return Copy of the value corresponding to success.
   */
  T operator*();

  /**
   *  Getter for the error, if result represents failure. Don't call it
   *  otherwise.
   *
   *  @return Copy of the error corresponding to failure.
   */
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
template <class T> const T *result<T>::borrow() { return &_value; }
template <class T> T result<T>::operator*() { return _value; }

template <class T> T *result<T>::operator->() { return &_value; }

template <class T> error_t result<T>::err() { return _error; }

#endif /* defined(__corrstat__result__) */
