#include "cube.hpp"

#include <emscripten/emscripten.h>

#include <cmath>
#include <random>

#include "../drawers/cube-drawer/geometry/cube-side-coords-range.hpp"
#include "graphics-math.hpp"
#include "ranges.hpp"

auto getCubeRotationForPosition(const CubePosition& pos, const Grid& grid)
    -> ModelRotation {
  auto pos3d = getPosition3dForCubePosition(pos, grid);

  // angle around X axis
  auto x_vector = pos3d;
  x_vector.y = 0;
  auto xAngleRad =
      (pos3d.y > 0 ? 1 : -1) * getAngleBetweenVectors(pos3d, x_vector);

  // angle around Y axis
  auto y_vector = pos3d;
  y_vector.x = 0;
  auto y_angle_rad =
      (pos3d.x > 0 ? -1 : 1) * getAngleBetweenVectors(pos3d, y_vector);
  y_angle_rad = pos3d.z < 0 ? M_PI - y_angle_rad : y_angle_rad;

  return {
      .x = std::round(normalizeDegrees(radToDeg(xAngleRad))),
      .y = std::round(normalizeDegrees(radToDeg(y_angle_rad))),
  };
}

auto getPosition3dForCubePosition(const CubePosition& pos, const Grid& grid)
    -> Point3D {
  auto vert_ratio = (pos.row + 0.5) / grid.rows_count;
  auto horiz_ratio = (pos.col + 0.5) / grid.cols_count;

  auto ranges = cube_side_coords_range.at(pos.side);

  auto dx = ranges.x[1] - ranges.x[0];
  auto dy = ranges.y[1] - ranges.y[0];
  auto dz = ranges.z[1] - ranges.z[0];

  switch (pos.side) {
    case ECubeSide::Front:
    case ECubeSide::Back:
      return {
          .x = ranges.x[0] + dx * horiz_ratio,
          .y = ranges.y[0] + dy * vert_ratio,
          .z = ranges.z[0],
      };
    case ECubeSide::Left:
    case ECubeSide::Right:
      return {
          .x = ranges.x[0],
          .y = ranges.y[0] + dy * vert_ratio,
          .z = ranges.z[0] + dz * horiz_ratio,
      };
    case ECubeSide::Up:
    case ECubeSide::Down:
      return {
          .x = ranges.x[0] + dx * horiz_ratio,
          .y = ranges.y[0],
          .z = ranges.z[0] + dz * vert_ratio,
      };
    default:
      emscripten_throw_string("Unknown cube side");
  }
}

auto getNextCubePositionAndDirection(const CubePosition& pos,
                                     EDirection direction, const Grid& grid)
    -> std::pair<CubePosition, EDirection> {
  auto next_pos = pos;
  auto next_direction = direction;

  switch (direction) {
    case EDirection::Up:
      next_pos.row += 1;
      break;
    case EDirection::Down:
      next_pos.row -= 1;
      break;
    case EDirection::Left:
      next_pos.col -= 1;
      break;
    case EDirection::Right:
      next_pos.col += 1;
      break;
  }

  // describe how cube sides adjust with each other when jumping from one side
  // to another

  // falling off the up edge
  if (next_pos.row >= grid.rows_count) {
    switch (next_pos.side) {
      case ECubeSide::Front:
        next_pos.side = ECubeSide::Up;
        next_pos.row = 0;
        break;
      case ECubeSide::Back:
        next_pos.side = ECubeSide::Up;
        next_direction = EDirection::Down;
        next_pos.col = grid.cols_count - next_pos.col - 1;
        next_pos.row = grid.rows_count - 1;
        break;
      case ECubeSide::Up:
        next_pos.side = ECubeSide::Back;
        next_direction = EDirection::Down;
        next_pos.col = grid.cols_count - next_pos.col - 1;
        next_pos.row = grid.rows_count - 1;
        break;
      case ECubeSide::Down:
        next_pos.side = ECubeSide::Front;
        next_pos.row = 0;
        break;
      case ECubeSide::Left:
        next_pos.side = ECubeSide::Up;
        next_direction = EDirection::Right;
        next_pos.row = grid.rows_count - next_pos.col - 1;
        next_pos.col = 0;
        break;
      case ECubeSide::Right:
        next_pos.side = ECubeSide::Up;
        next_direction = EDirection::Left;
        next_pos.row = next_pos.col;
        next_pos.col = grid.cols_count - 1;
        break;
    }
  }

  // falling off the down edge
  if (next_pos.row < 0) {
    switch (next_pos.side) {
      case ECubeSide::Front:
        next_pos.side = ECubeSide::Down;
        next_pos.row = grid.cols_count - 1;
        break;
      case ECubeSide::Back:
        next_pos.side = ECubeSide::Down;
        next_direction = EDirection::Up;
        next_pos.col = grid.cols_count - next_pos.col - 1;
        next_pos.row = 0;
        break;
      case ECubeSide::Up:
        next_pos.side = ECubeSide::Front;
        next_pos.row = grid.rows_count - 1;
        break;
      case ECubeSide::Down:
        next_pos.side = ECubeSide::Back;
        next_direction = EDirection::Up;
        next_pos.col = grid.cols_count - next_pos.col - 1;
        next_pos.row = 0;
        break;
      case ECubeSide::Left:
        next_pos.side = ECubeSide::Down;
        next_direction = EDirection::Right;
        next_pos.row = next_pos.col;
        next_pos.col = 0;
        break;
      case ECubeSide::Right:
        next_pos.side = ECubeSide::Down;
        next_direction = EDirection::Left;
        next_pos.row = grid.rows_count - next_pos.col - 1;
        next_pos.col = grid.cols_count - 1;
        break;
    }
  }

  // falling off the right edge
  if (next_pos.col >= grid.cols_count) {
    switch (next_pos.side) {
      case ECubeSide::Front:
        next_pos.side = ECubeSide::Right;
        next_pos.col = 0;
        break;
      case ECubeSide::Back:
        next_pos.side = ECubeSide::Left;
        next_pos.col = 0;
        break;
      case ECubeSide::Up:
        next_pos.side = ECubeSide::Right;
        next_direction = EDirection::Down;
        next_pos.col = next_pos.row;
        next_pos.row = grid.rows_count - 1;
        break;
      case ECubeSide::Down:
        next_pos.side = ECubeSide::Right;
        next_direction = EDirection::Up;
        next_pos.col = grid.rows_count - next_pos.row - 1;
        next_pos.row = 0;
        break;
      case ECubeSide::Left:
        next_pos.side = ECubeSide::Front;
        next_pos.col = 0;
        break;
      case ECubeSide::Right:
        next_pos.side = ECubeSide::Back;
        next_pos.col = 0;
        break;
    }
  }

  // falling off the left edge
  if (next_pos.col < 0) {
    switch (next_pos.side) {
      case ECubeSide::Front:
        next_pos.side = ECubeSide::Left;
        next_pos.col = grid.cols_count - 1;
        break;
      case ECubeSide::Back:
        next_pos.side = ECubeSide::Right;
        next_pos.col = grid.cols_count - 1;
        break;
      case ECubeSide::Up:
        next_pos.side = ECubeSide::Left;
        next_direction = EDirection::Down;
        next_pos.col = grid.cols_count - next_pos.row - 1;
        next_pos.row = grid.rows_count - 1;
        break;
      case ECubeSide::Down:
        next_pos.side = ECubeSide::Left;
        next_direction = EDirection::Up;
        next_pos.col = next_pos.row;
        next_pos.row = 0;
        break;
      case ECubeSide::Left:
        next_pos.side = ECubeSide::Back;
        next_pos.col = grid.cols_count - 1;
        break;
      case ECubeSide::Right:
        next_pos.side = ECubeSide::Front;
        next_pos.col = grid.cols_count - 1;
        break;
    }
  }

  return {next_pos, next_direction};
}

auto getRandomCubePosition(const Cube& cube) -> CubePosition {
  std::random_device device;
  std::mt19937 engine(device());

  std::uniform_int_distribution<std::mt19937::result_type> side_dist(
      0, cube.sides.size() - 1);

  std::uniform_int_distribution<int> row_dist(0, cube.grid.rows_count - 1);
  std::uniform_int_distribution<int> col_dist(0, cube.grid.cols_count - 1);

  return {
      .side = static_cast<ECubeSide>(side_dist(engine)),
      .row = row_dist(engine),
      .col = col_dist(engine),
  };
}
