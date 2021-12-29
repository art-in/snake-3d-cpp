#pragma once

#include "../helpers/cube.hpp"
#include "../helpers/ranges.hpp"
#include "../models/GameState.hpp"
#include "../models/angles.hpp"
#include "../models/ranges.hpp"

void autoRotateLoop(GameState* state);
auto makeRotationStep(Degrees current_angle, Degrees target_angle) -> double;
auto getRotationDirection(Degrees from, Degrees to) -> int;
