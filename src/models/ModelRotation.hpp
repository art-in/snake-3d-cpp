#pragma once

#include "angles.hpp"

struct ModelRotation {
  Degrees x{};
  Degrees y{};
};

auto operator!=(const ModelRotation& lhs, const ModelRotation& rhs) -> bool;