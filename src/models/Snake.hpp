#pragma once

#include <chrono>
#include <list>
#include <optional>

#include "CubePosition.hpp"
#include "ECubeSide.hpp"
#include "EDirection.hpp"

struct Snake {
  using steady_clock = std::chrono::steady_clock;
  using duration_ms = std::chrono::duration<double, std::milli>;

  std::optional<std::chrono::time_point<steady_clock>> last_move_time{};
  std::list<CubePosition> parts{{ECubeSide::Front, 0, 0}};
  EDirection direction{EDirection::Right};
  duration_ms move_period{150};
  bool is_crashed{false};
};
