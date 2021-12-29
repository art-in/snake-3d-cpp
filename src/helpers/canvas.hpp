#pragma once

#include <emscripten/val.h>

#include <string>

#include "../models/Size.hpp"

auto getCanvasFontString(uint32_t size = 8, const std::string& family = "Arial",
                         const std::string& unit = "px",
                         const std::string& weight = "") -> std::string;

void resizeCanvas(emscripten::val canvas, Size css_size, double pixel_ratio);

auto measureCanvasText(const emscripten::val& canvas_ctx_2d,
                       const std::string& text) -> Size;
