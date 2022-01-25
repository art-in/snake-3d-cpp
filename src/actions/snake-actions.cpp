#include "snake-actions.hpp"

#include <chrono>

#include "../helpers/cube.hpp"
#include "../helpers/direction.hpp"

const double SNAKE_MOVE_PERIOD_MULTIPLIER = 0.05;  // higher is faster
const bool MOVE_SNAKE = true;                      // for debug

void moveSnakeLoop(GameState* state) {
  auto& snake = state->snake;

  const auto now = std::chrono::steady_clock::now();

  if (MOVE_SNAKE && state->status == EGameStatus::InGame &&
      (!snake.last_move_time.has_value() ||
       now - snake.last_move_time.value() >= snake.move_period)) {
    moveSnake(state);
    snake.last_move_time = now;
  }
}

void moveSnake(GameState* state) {
  auto& scene = state->scene;
  auto& snake = state->snake;

  // instead of moving each snake part one step ahead, move tail to new head
  const auto head = snake.parts.front();
  const auto tail = snake.parts.back();

  scene.cube.sides[tail.side].needs_redraw = true;
  snake.parts.pop_back();

  auto [newHead, newDirection] =
      getNextCubePositionAndDirection(head, snake.direction, scene.cube.grid);

  snake.parts.push_front(newHead);
  snake.direction = newDirection;

  scene.cube.sides[newHead.side].needs_redraw = true;

  checkForApple(state);
  checkCrash(state);
}

void setSnakeDirection(GameState* state, EDirection direction) {
  if (state->snake.direction == getOppositeDirection(direction)) {
    return;
  }

  state->snake.direction = direction;
}

void checkForApple(GameState* state) {
  auto& snake = state->snake;
  auto& apples = state->apples;

  const auto& head = snake.parts.front();
  const auto& tail = snake.parts.back();

  auto it = apples.find(head);
  if (it != apples.end()) {
    apples.erase(it);
    snake.parts.push_back(tail);

    snake.move_period *= 1 - SNAKE_MOVE_PERIOD_MULTIPLIER;
  }
}

void checkCrash(GameState* state) {
  auto& snake = state->snake;
  const auto& stones = state->stones;

  const auto& head = snake.parts.front();

  // crash on stone
  if (stones.count(head) > 0) {
    snake.is_crashed = true;
  }

  // crash on tail
  if (snake.parts.size() > 3) {
    auto it = std::next(snake.parts.begin(), 3);
    for (; it != snake.parts.end(); ++it) {
      if (*it == head) {
        snake.is_crashed = true;
        break;
      }
    }
  }
}