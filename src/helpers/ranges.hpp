#pragma once

#include "../models/angles.hpp"
#include "../models/ranges.hpp"

auto normalizeByCircularRange(double value, double start, double end) -> double;
auto normalizeDegrees(Degrees degrees) -> Degrees;
auto projectToRange(double a, const Range& rangeA, const Range& rangeB)
    -> double;
