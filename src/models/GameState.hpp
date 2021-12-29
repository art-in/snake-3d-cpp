#pragma once

#include <set>

#include "CubePosition.hpp"
#include "EGameStatus.hpp"
#include "Scene.hpp"
#include "Snake.hpp"

struct GameState {
  Scene scene;

  Snake snake;
  std::set<CubePosition> apples{};
  std::set<CubePosition> stones{};

  EGameStatus status{EGameStatus::Welcome};
};
