#include "CubePosition.hpp"

#include <tuple>

auto operator<(const CubePosition& lhs, const CubePosition& rhs) -> bool {
  return std::tie(lhs.side, lhs.row, lhs.col) <
         std::tie(rhs.side, rhs.row, rhs.col);
}
