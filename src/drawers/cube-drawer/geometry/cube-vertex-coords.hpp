#pragma once

#include <GLES2/gl2.h>

#include <array>

#include "../../../models/ECubeSide.hpp"

static const float front = static_cast<float>(ECubeSide::Front);
static const float back = static_cast<float>(ECubeSide::Back);
static const float up = static_cast<float>(ECubeSide::Up);
static const float down = static_cast<float>(ECubeSide::Down);
static const float left = static_cast<float>(ECubeSide::Left);
static const float right = static_cast<float>(ECubeSide::Right);

static const std::array<GLfloat, 144> cube_vertex_coords{
    // clang-format off
  front, -0.5, -0.5,   0.5,
  front,  0.5, -0.5,   0.5,
  front, -0.5,  0.5,   0.5,
  front, -0.5,  0.5,   0.5,
  front,  0.5, -0.5,   0.5,
  front,  0.5,  0.5,   0.5,

  back, -0.5,  -0.5,  -0.5,
  back, -0.5,   0.5,  -0.5,
  back,  0.5,  -0.5,  -0.5,
  back, -0.5,   0.5,  -0.5,
  back,  0.5,   0.5,  -0.5,
  back,  0.5,  -0.5,  -0.5,

  up,   -0.5,   0.5,  -0.5,
  up,   -0.5,   0.5,   0.5,
  up,    0.5,   0.5,  -0.5,
  up,   -0.5,   0.5,   0.5,
  up,    0.5,   0.5,   0.5,
  up,    0.5,   0.5,  -0.5,

  down, -0.5,  -0.5,  -0.5,
  down,  0.5,  -0.5,  -0.5,
  down, -0.5,  -0.5,   0.5,
  down, -0.5,  -0.5,   0.5,
  down,  0.5,  -0.5,  -0.5,
  down,  0.5,  -0.5,   0.5,

  left, -0.5,  -0.5,  -0.5,
  left, -0.5,  -0.5,   0.5,
  left, -0.5,   0.5,  -0.5,
  left, -0.5,  -0.5,   0.5,
  left, -0.5,   0.5,   0.5,
  left, -0.5,   0.5,  -0.5,

  right, 0.5,  -0.5,  -0.5,
  right, 0.5,   0.5,  -0.5,
  right, 0.5,  -0.5,   0.5,
  right, 0.5,  -0.5,   0.5,
  right, 0.5,   0.5,  -0.5,
  right, 0.5,   0.5,   0.5,
    // clang-format on
};
