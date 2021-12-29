#pragma once

#include <emscripten/val.h>

#include <optional>

#include "ECubeSide.hpp"

struct CubeSide {
  std::optional<emscripten::val> canvas;
  std::optional<emscripten::val> ctx;

  ECubeSide type{};

  bool needs_redraw{true};
  bool needs_update_on_cube{true};
};
