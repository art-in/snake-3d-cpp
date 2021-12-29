#pragma once

#include <iostream>

#include "ECubeSide.hpp"

struct CubePosition {
  ECubeSide side{};
  int row{};
  int col{};

  auto operator==(const CubePosition& other) const -> bool = default;
};

auto operator<(const CubePosition& lhs, const CubePosition& rhs) -> bool;
