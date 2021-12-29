#include "ranges.hpp"

#include <cmath>

// normalizes any number to an arbitrary range
// by assuming the range wraps around when going below min or above max
auto normalizeByCircularRange(double value, double start, double end)
    -> double {
  auto width = end - start;
  auto offset_value = value - start;  // value relative to 0

  return offset_value - std::floor(offset_value / width) * width + start;
  // + start to reset back to start of original range
}

auto normalizeDegrees(Degrees degrees) -> Degrees {
  return normalizeByCircularRange(degrees, -180, 181);
}

auto projectToRange(double a, const Range& rangeA, const Range& rangeB)
    -> double {
  auto da = rangeA[1] - rangeA[0];
  auto dn = a - rangeA[0];
  auto ratio = dn / da;

  auto db = rangeB[1] - rangeB[0];
  auto b = rangeB[0] + db * ratio;

  return b;
}
