#include "../data.h"

static short sgn(double val) { return (0 < val) - (val < 0); }

extern "C" double exec(cst::data data) {
  long num = 0;
  for (cst::ul i = 1; i < data.size(); ++i) {
    for (cst::ul j = 0; j < i - 1; ++j) {
      num += sgn(data[i][0] - data[j][0]) * sgn(data[i][1] - data[j][1]);
    }
  }

  double denom = data.size() * (data.size() - 1) / 2;

  return num / denom;
}
