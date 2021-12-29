#pragma once

#include "../models/GameState.hpp"

void initGameState(GameState* state);
void updateGameStateLoop(GameState* state);
void plantObjects(GameState* state);
void startOrPauseGame(GameState* state);