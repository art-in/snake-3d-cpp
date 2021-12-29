#pragma once

#include <array>
#include <cmath>

#include "../models/Point3D.hpp"
#include "../models/angles.hpp"

using Matrix4 = std::array<float, 4 * 4>;
using Vec3 = std::array<float, 3>;

static constexpr Degrees FULL_CIRCLE = 360;
static constexpr Degrees HALF_CIRCLE = FULL_CIRCLE / 2;

constexpr auto radToDeg(const Radians r) -> Degrees {
  return (r * HALF_CIRCLE) / M_PI;
}
constexpr auto degToRad(const Degrees d) -> Radians {
  return (d * M_PI) / HALF_CIRCLE;
}

auto multiply(Matrix4 a, Matrix4 b) -> Matrix4;

auto lookAt(Vec3 camera_pos, Vec3 target, Vec3 up) -> Matrix4;

auto inverse(Matrix4 m) -> Matrix4;

auto xRotate(Matrix4 m, Radians angle) -> Matrix4;
auto yRotate(Matrix4 m, Radians angle) -> Matrix4;

auto perspective(Radians field_of_view, float aspect, float near, float far)
    -> Matrix4;

auto getAngleBetweenVectors(const Point3D& a, const Point3D& b) -> Radians;