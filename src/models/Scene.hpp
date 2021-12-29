#pragma once

#include <emscripten/val.h>

#include <optional>

#include "Cube.hpp"

struct Scene {
  std::optional<emscripten::val> canvas;
  std::optional<emscripten::val> ctx;

  Cube cube;
};
