#pragma once

#include <GLES2/gl2.h>

#include <map>
#include <optional>
#include <vector>

#include "CubeSide.hpp"
#include "ECameraMode.hpp"
#include "Grid.hpp"
#include "ModelRotation.hpp"
#include "Point2D.hpp"

struct Cube {
  std::optional<GLuint> program{};
  std::optional<GLint> matrix_uniform_location{};
  std::vector<GLuint> textures;

  ModelRotation current_rotation;
  ModelRotation target_rotation;

  ECameraMode camera_mode{ECameraMode::Overview};

  bool mouse_is_dragging{false};
  std::optional<Point2D> mouse_pos;

  bool needs_redraw{false};

  static const int GRID_SIZE = 16;
  Grid grid{.rows_count = GRID_SIZE, .cols_count = GRID_SIZE};

  std::map<ECubeSide, CubeSide> sides{
      {ECubeSide::Front, CubeSide{.type = ECubeSide::Front}},
      {ECubeSide::Back, CubeSide{.type = ECubeSide::Back}},
      {ECubeSide::Up, CubeSide{.type = ECubeSide::Up}},
      {ECubeSide::Down, CubeSide{.type = ECubeSide::Down}},
      {ECubeSide::Left, CubeSide{.type = ECubeSide::Left}},
      {ECubeSide::Right, CubeSide{.type = ECubeSide::Right}},
  };
};
