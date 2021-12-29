#pragma once

#include <GLES2/gl2.h>

#include <array>

static const std::array<GLfloat, 72> cube_texture_coords{
    // clang-format off
  // front side
  0, 1,
  1, 1,
  0, 0,
  0, 0,
  1, 1,
  1, 0,

  // back side
  1, 1,
  1, 0,
  0, 1,
  1, 0,
  0, 0,
  0, 1,

  // up side
  0, 0,
  0, 1,
  1, 0,
  0, 1,
  1, 1,
  1, 0,

  // down side
  0, 1,
  1, 1,
  0, 0,
  0, 0,
  1, 1,
  1, 0,

  // left side
  0, 1,
  1, 1,
  0, 0,
  1, 1,
  1, 0,
  0, 0,

  // right side
  1, 1,
  1, 0,
  0, 1,
  0, 1,
  1, 0,
  0, 0,
    // clang-format on
};
