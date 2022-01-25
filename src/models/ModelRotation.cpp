#include "ModelRotation.hpp"

auto operator!=(const ModelRotation& lhs, const ModelRotation& rhs) -> bool {
  return lhs.x != rhs.x || lhs.y != rhs.y;
}
