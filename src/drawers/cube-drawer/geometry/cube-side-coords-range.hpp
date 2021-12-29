#pragma once

#include <map>

#include "../../../helpers/ranges.hpp"
#include "../../../models/ECubeSide.hpp"

struct CubeSideRangesByAxis {
  Range x;
  Range y;
  Range z;
};

// NOLINTNEXTLINE(cert-err58-cpp)
static const std::map<ECubeSide, CubeSideRangesByAxis> cube_side_coords_range{
    {ECubeSide::Front, {.x = {-0.5, 0.5}, .y = {-0.5, 0.5}, .z = {0.5, 0.5}}},
    {ECubeSide::Left, {.x = {-0.5, -0.5}, .y = {-0.5, 0.5}, .z = {-0.5, 0.5}}},
    {ECubeSide::Right, {.x = {0.5, 0.5}, .y = {-0.5, 0.5}, .z = {0.5, -0.5}}},
    {ECubeSide::Back, {.x = {0.5, -0.5}, .y = {-0.5, 0.5}, .z = {-0.5, -0.5}}},
    {ECubeSide::Up, {.x = {-0.5, 0.5}, .y = {0.5, 0.5}, .z = {0.5, -0.5}}},
    {ECubeSide::Down, {.x = {-0.5, 0.5}, .y = {-0.5, -0.5}, .z = {-0.5, 0.5}}}};
