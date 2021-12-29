#pragma once

#include "../../helpers/graphics-math.hpp"
#include "../../models/GameState.hpp"

void initCubeDrawer(GameState* state);
void drawCubeLoop(GameState* state);
void drawCube(GameState* state, const Matrix4& matrix);
