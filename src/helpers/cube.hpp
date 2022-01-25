#pragma once

#include <utility>

#include "../models/Cube.hpp"
#include "../models/CubePosition.hpp"
#include "../models/ECubeSide.hpp"
#include "../models/EDirection.hpp"
#include "../models/Grid.hpp"
#include "../models/ModelRotation.hpp"
#include "../models/Point3D.hpp"

auto getPosition3dForCubePosition(const CubePosition& pos, const Grid& grid)
    -> Point3D;

auto getCubeRotationForPosition(const CubePosition& pos, const Grid& grid)
    -> ModelRotation;

auto getNextCubePositionAndDirection(const CubePosition& pos,
                                     EDirection direction, const Grid& grid)
    -> std::pair<CubePosition, EDirection>;

auto getRandomCubePosition(const Cube& cube) -> CubePosition;