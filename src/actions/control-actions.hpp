#pragma once

#include <emscripten/html5.h>

#include <string>

#include "../models/GameState.hpp"

void onKeyDown(GameState* state, const std::string& key_code);
void onMouseDown(GameState* state);
void onMouseUp(GameState* state);
void onMouseMove(GameState* state, Point2D mouse_pos);
