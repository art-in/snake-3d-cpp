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

  std::set<CubePosition> object_positions;

  // plant snake
  state->snake = Snake{};
  object_positions.insert(*state->snake.parts.begin());

  // plant apples
  state->apples.clear();

  while (state->apples.size() < APPLES_COUNT) {
    auto pos = getRandomCubePosition(scene.cube);

    // do not plant above other objects
    if (object_positions.count(pos) == 0) {
      state->apples.insert(pos);
      object_positions.insert(pos);
    }
  }

  // plant stones
  state->stones.clear();

  while (state->stones.size() < STONES_COUNT) {
    auto pos = getRandomCubePosition(scene.cube);

    if (object_positions.count(pos) == 0) {
      state->stones.insert(pos);
      object_positions.insert(pos);
    }
  }

  for (auto& [type, side] : scene.cube.sides) {
    side.needs_redraw = true;
  }
}

void startOrPauseGame(GameState* state) {
  switch (state->status) {
    case EGameStatus::Welcome:
    case EGameStatus::Paused:
      state->status = EGameStatus::InGame;
      break;
    case EGameStatus::Win:
    case EGameStatus::Fail:
      plantObjects(state);
      state->status = EGameStatus::InGame;
      break;
    case EGameStatus::InGame:
      state->status = EGameStatus::Paused;
      break;
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
