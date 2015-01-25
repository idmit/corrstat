#include <vector>

extern "C" double exec(std::vector<std::vector<double>> samples) {
  double acc = 0;
  for (auto num : samples[0]) {
    acc += num;
  }

  return acc / samples[0].size();
}
