#include "canvas.hpp"

#include <cmath>
#include <sstream>

auto getCanvasFontString(uint32_t size, const std::string& family,
                         const std::string& unit, const std::string& weight)
    -> std::string {
  std::ostringstream os;

  if (!weight.empty()) {
    os << weight << ' ';
  }

  os << size << unit << ' ' << family;

  return os.str();
}

void resizeCanvas(emscripten::val canvas, Size css_size, double pixel_ratio) {
  canvas.set("width", css_size.width * pixel_ratio);
  canvas.set("height", css_size.height * pixel_ratio);

  canvas["style"].set("width", std::to_string(css_size.width) + "px");
  canvas["style"].set("height", std::to_string(css_size.height) + "px");
}

auto measureCanvasText(const emscripten::val& canvas_ctx_2d,
                       const std::string& text) -> Size {
  auto text_size = canvas_ctx_2d.call<emscripten::val, const std::string&>(
      "measureText", text);

  return {
      .width = std::ceil(text_size["width"].as<double>()),
      .height = std::ceil(text_size["actualBoundingBoxAscent"].as<double>())};
}
