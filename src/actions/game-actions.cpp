#include "game-actions.hpp"

#include <algorithm>
#include <set>
#include <vector>

#include "../helpers/cube.hpp"
#include "cube-actions.hpp"
#include "snake-actions.hpp"

const int APPLES_COUNT = 10;
const int STONES_COUNT = 10;

void initGameState(GameState* state) {
  state->status = EGameStatus::Welcome;
  plantObjects(state);
}

void updateGameStateLoop(GameState* state) {
  moveSnakeLoop(state);
  autoRotateLoop(state);

  auto& cube = state->scene.cube;

  if (state->status == EGameStatus::InGame) {
    if (state->snake.is_crashed) {
      state->status = EGameStatus::Fail;
      cube.camera_mode = ECameraMode::Overview;
      std::for_each(cube.sides.begin(), cube.sides.end(),
                    [](std::pair<const ECubeSide, CubeSide>& p) {
                      p.second.needs_redraw = true;
                    });
    }

    if (state->apples.empty()) {
      state->status = EGameStatus::Win;
      cube.camera_mode = ECameraMode::Overview;
      std::for_each(cube.sides.begin(), cube.sides.end(),
                    [](std::pair<const ECubeSide, CubeSide>& p) {
                      p.second.needs_redraw = true;
                    });
    }
  }
}

void plantObjects(GameState* state) {
  auto& scene = state->scene;
  state->snake = Snake{};

  std::set<CubePosition> object_positions;

  // plant apples
  state->apples.clear();

  for (int i = 0; i < APPLES_COUNT; i++) {
    auto pos = getRandomCubePosition(scene.cube);
    state->apples.insert(pos);
    object_positions.insert(pos);
  }

  // plant stones
  state->stones.clear();

  for (int i = 0; i < STONES_COUNT; i++) {
    auto pos = getRandomCubePosition(scene.cube);

    // do not place stones above other objects eg. apples
    if (object_positions.count(pos) == 0) {
      state->stones.insert(pos);
    }
  }

  for (auto& [type, side] : scene.cube.sides) {
    side.needs_redraw = true;
  }
}

void startOrPauseGame(GameState* state) {
  if (state->status == EGameStatus::Welcome ||
      state->status == EGameStatus::Paused) {
    state->status = EGameStatus::InGame;
  } else if (state->status == EGameStatus::Win ||
             state->status == EGameStatus::Fail) {
    plantObjects(state);
    state->status = EGameStatus::InGame;
  } else if (state->status == EGameStatus::InGame) {
    state->status = EGameStatus::Paused;
  }

  if (state->status == EGameStatus::InGame) {
    state->scene.cube.camera_mode = ECameraMode::FollowSnake;
  } else {
    state->scene.cube.camera_mode = ECameraMode::Overview;
  }

  for (auto& [type, side] : state->scene.cube.sides) {
    side.needs_redraw = true;
  }
}
