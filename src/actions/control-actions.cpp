#include "control-actions.hpp"

#include "../drawers/scene-drawer.hpp"
#include "../helpers/direction.hpp"
#include "../helpers/ranges.hpp"
#include "../models/ECameraMode.hpp"
#include "../models/EDirection.hpp"
#include "../models/EGameStatus.hpp"
#include "game-actions.hpp"
#include "snake-actions.hpp"

void onKeyDown(GameState* state, const std::string& key_code) {
  std::optional<EDirection> direction;

  if (key_code == "ArrowUp" || key_code == "KeyW") {
    direction = EDirection::Up;
  } else if (key_code == "ArrowDown" || key_code == "KeyS") {
    direction = EDirection::Down;
  } else if (key_code == "ArrowLeft" || key_code == "KeyA") {
    direction = EDirection::Left;
  } else if (key_code == "ArrowRight" || key_code == "KeyD") {
    direction = EDirection::Right;
  } else if (key_code == "Space" || key_code == "Enter") {
    startOrPauseGame(state);
  }

  if (direction.has_value()) {
    const auto& head = state->snake.parts.front();
    const auto& grid = state->scene.cube.grid;

    // adjust direction per current camera rotation
    if ((head.side == ECubeSide::Up && head.row >= grid.rows_count / 2) ||
        (head.side == ECubeSide::Down && head.row < grid.rows_count / 2)) {
      direction = getOppositeDirection(direction.value());
    }

    setSnakeDirection(state, direction.value());
  }
}

void onMouseDown(GameState* state) {
  if (state->status != EGameStatus::InGame) {
    auto& cube = state->scene.cube;

    cube.mouse_is_dragging = true;
    cube.camera_mode = ECameraMode::ManualControl;
  }
}

void onMouseUp(GameState* state) {
  auto& cube = state->scene.cube;

  cube.mouse_is_dragging = false;
  cube.mouse_pos.reset();
}

void onMouseMove(GameState* state, Point2D mouse_pos) {
  auto& cube = state->scene.cube;

  if (!cube.mouse_is_dragging) {
    return;
  }

  if (cube.mouse_pos.has_value()) {
    auto dx = cube.mouse_pos->x - mouse_pos.x;
    auto dy = cube.mouse_pos->y - mouse_pos.y;

    constexpr double ROTATION_VELOCITY = 0.25;
    cube.target_rotation.x -= dy * ROTATION_VELOCITY;
    cube.target_rotation.y -= dx * ROTATION_VELOCITY;

    cube.target_rotation.x = normalizeDegrees(cube.target_rotation.x);
    cube.target_rotation.y = normalizeDegrees(cube.target_rotation.y);

    cube.current_rotation = cube.target_rotation;

    cube.needs_redraw = true;
  }

  cube.mouse_pos = mouse_pos;
}
