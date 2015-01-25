//
//  data.h
//  corrstat
//
//  Created by Иван Дмитриевский on 22/01/15.
//  Copyright (c) 2015 Иван Дмитриевский. All rights reserved.
//

#ifndef __corrstat__data__
#define __corrstat__data__

#include <vector>

namespace cst {

typedef std::vector<double> point;
typedef unsigned long ul;

class data {
public:
  data(ul dim, ul size = 0);
  bool (*eq)(point l, point r);
  bool (*lt)(point l, point r);
  bool (*gt)(point l, point r);
  short (*cmp)(point l, point r);

private:
  const ul _dim;
  std::vector<point> _raw;
  static bool _eq(point l, point r);
  static bool _lt(point l, point r);
  static bool _gt(point l, point r);
  static short _cmp(point l, point r);
};
}
#endif /* defined(__corrstat__data__) */
