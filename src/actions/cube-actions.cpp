#include "cube-actions.hpp"

#include <emscripten.h>

#include <algorithm>
#include <cmath>

const double AUTO_ROTATION_STEP_MIN = 0.5;
const double AUTO_ROTATION_STEP_MAX = 10;
const Range AUTO_ROTATION_STEP_RANGE{
    AUTO_ROTATION_STEP_MIN,
    AUTO_ROTATION_STEP_MAX,
};
const Range AUTO_ROTATION_ANGLE_RANGE{0, 180};

void autoRotateLoop(GameState* state) {
  auto& scene = state->scene;
  auto& cube = scene.cube;

  auto& current_rotation = cube.current_rotation;
  auto& target_rotation = cube.target_rotation;

  if (cube.camera_mode == ECameraMode::Overview) {
    target_rotation.y = normalizeDegrees(target_rotation.y - 0.3);
  }

  if (cube.camera_mode == ECameraMode::FollowSnake) {
    const auto& head = state->snake.parts.front();
    cube.target_rotation = getCubeRotationForPosition(head, cube.grid);
  }

  if (cube.current_rotation.x != cube.target_rotation.x ||
      cube.current_rotation.y != cube.target_rotation.y) {
    cube.needs_redraw = true;
  }

  if (current_rotation.x != target_rotation.x) {
    current_rotation.x =
        makeRotationStep(current_rotation.x, target_rotation.x);
  }

  if (current_rotation.y != target_rotation.y) {
    current_rotation.y =
        makeRotationStep(current_rotation.y, target_rotation.y);
  }
}

auto makeRotationStep(Degrees current_angle, Degrees target_angle) -> double {
  const auto angle_diff =
      std::min({std::abs(current_angle - target_angle),
                std::abs(current_angle - target_angle - 360),
                std::abs(current_angle - target_angle + 360)});

  if (angle_diff < AUTO_ROTATION_STEP_MIN) {
    return target_angle;
  }

  auto rotation_step = projectToRange(angle_diff, AUTO_ROTATION_ANGLE_RANGE,
                                      AUTO_ROTATION_STEP_RANGE);
  auto nextCurrentRotation =
      current_angle +
      rotation_step * getRotationDirection(current_angle, target_angle);
  return normalizeDegrees(nextCurrentRotation);
}

auto getRotationDirection(Degrees from, Degrees to) -> int {
  if (from == to) {
    return 0;
  }

  if (from >= 0 && to >= 0) {
    return from > to ? -1 : 1;
  }

  if (from <= 0 && to <= 0) {
    return from > to ? -1 : 1;
  }

  if (from >= 0 && to <= 0) {
    return (from - to <= 180) ? -1 : 1;
  }

  if (from <= 0 && to >= 0) {
    return (-from + to < 180) ? 1 : -1;
  }

  emscripten_throw_string("Should not be here");
}
