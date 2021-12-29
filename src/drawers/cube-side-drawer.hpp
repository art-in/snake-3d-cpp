#pragma once

#include "../models/ECubeSide.hpp"
#include "../models/GameState.hpp"

void initCubeSideDrawer(GameState* state, ECubeSide side);
void drawCubeSideLoop(GameState* state, ECubeSide cubeSide);