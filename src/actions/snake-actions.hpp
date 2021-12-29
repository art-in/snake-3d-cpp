#pragma once

#include "../models/GameState.hpp"

void moveSnakeLoop(GameState* state);
void moveSnake(GameState* state);
void setSnakeDirection(GameState* state, EDirection direction);
void checkForApple(GameState* state);
void checkCrash(GameState* state);
