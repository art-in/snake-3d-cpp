#pragma once

#include <emscripten/val.h>

#include "../models/GameState.hpp"

void initSceneDrawer(GameState* state, emscripten::val canvas);
void drawSceneLoop(GameState* state);
