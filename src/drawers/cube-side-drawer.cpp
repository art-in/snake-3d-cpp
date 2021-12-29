#include "cube-side-drawer.hpp"

#include <emscripten/val.h>

#include <cmath>
#include <string>

#include "../helpers/assert.hpp"
#include "../helpers/canvas.hpp"

// cube sides are drawn in 2D context and passed as textures to 3D cube.
// this is not very performant approach, since we need to upload entire side
// image when something small changes on it. faster wound be to upload object
// positions only and draw them as separate 3D entities, textures untouched.
// I've dodged this approach because I guess it would be harder to code, while
// I want it to be as basic as possible without diving into 3D coding hell
// (for that I would chose some 3D library) ie. need to calculate 3D positions
// for all objects, apply different textures for different objects (snake,
// stones, apples, status overlays), etc.
//
// using dynamic binding to 2D context API instead of static bindings. for
// static bindings only option I see in emscripten is SDL API, but it's very
// different from web API and I want to stick to web API as close as possible
void initCubeSideDrawer(GameState* state, ECubeSide side) {
  auto document = emscripten::val::global("document");
  auto canvas =
      document.call<emscripten::val, std::string>("createElement", "canvas");

  canvas.set("width", std::pow(2, 9));
  canvas.set("height", std::pow(2, 9));

  auto ctx = canvas.call<emscripten::val, std::string>("getContext", "2d");

  state->scene.cube.sides[side].canvas = canvas;
  state->scene.cube.sides[side].ctx = ctx;
  state->scene.cube.sides[side].needs_redraw = true;
  state->scene.cube.sides[side].needs_update_on_cube = true;
}

void drawCubeSideLoop(GameState* state, ECubeSide side_type) {
  const auto& grid = state->scene.cube.grid;

  auto& side = state->scene.cube.sides[side_type];
  if (!side.needs_redraw) {
    return;
  }

  ASSERT(side.canvas.has_value());
  ASSERT(side.ctx.has_value());

  const auto& canvas = side.canvas.value();
  auto& ctx = side.ctx.value();

  const auto width = canvas["width"].as<double>();
  const auto height = canvas["height"].as<double>();

  ctx.call<void>("clearRect", 0, 0, width, height);

  ctx.set("globalAlpha", 1);
  ctx.set("fillStyle", "white");
  ctx.call<void>("fillRect", 0, 0, width, height);

  // draw grid
  const auto cellWidth = width / grid.cols_count;
  const auto cellHeight = height / grid.rows_count;

  for (int i = 1; i < grid.cols_count; ++i) {
    const auto x = i * cellWidth;
    ctx.call<void>("moveTo", x, 0);
    ctx.call<void>("lineTo", x, height);
  }

  for (int i = 1; i < grid.rows_count; ++i) {
    const auto y = i * cellHeight;
    ctx.call<void>("moveTo", 0, y);
    ctx.call<void>("lineTo", width, y);
  }

  ctx.set("lineWidth", 1);
  ctx.call<void>("stroke");

  // draw snake
  ctx.set("fillStyle", "red");
  for (const auto& part : state->snake.parts) {
    if (part.side == side_type) {
      ctx.call<void>("fillRect", part.col * cellWidth,
                     height - part.row * cellHeight - cellHeight, cellWidth,
                     cellHeight);
    }
  }

  // draw apples
  ctx.set("fillStyle", "green");
  for (const auto& apple : state->apples) {
    if (apple.side == side_type) {
      ctx.call<void>("fillRect", apple.col * cellWidth,
                     height - apple.row * cellHeight - cellHeight, cellWidth,
                     cellHeight);
    }
  }

  // draw stones
  ctx.set("fillStyle", "black");
  for (const auto& stone : state->stones) {
    if (stone.side == side_type) {
      ctx.call<void>("fillRect", stone.col * cellWidth,
                     height - stone.row * cellHeight - cellHeight, cellWidth,
                     cellHeight);
    }
  }

  // draw status overlay
  if (state->status != EGameStatus::InGame) {
    static const auto overlay_height = 200;
    static const auto overlay_width = 400;
    static const auto overlay_padding = 30;

    const auto overlay_horizontal_margin = (width - overlay_width) / 2;
    const auto overlay_vertical_margin = (height - overlay_height) / 2;

    ctx.set("globalAlpha", 0.7);
    ctx.set("fillStyle", "white");
    ctx.call<void>("fillRect", overlay_horizontal_margin,
                   overlay_vertical_margin, overlay_width, overlay_height);

    ctx.set("lineWidth", 3);
    ctx.set("strokeStyle", "black");
    ctx.call<void>("strokeRect", overlay_horizontal_margin,
                   overlay_vertical_margin, overlay_width, overlay_height);

    ctx.set("fillStyle", "black");
    static const auto title_font =
        getCanvasFontString(70, "Consolas", "px", "bold");
    ctx.set("font", title_font);
    const std::string title = state->status == EGameStatus::Paused ? "PAUSED"
                              : state->status == EGameStatus::Win  ? "WIN"
                              : state->status == EGameStatus::Fail ? "FAIL"
                                                                   : "SNAKE 3D";

    const auto header_size = measureCanvasText(ctx, title);
    ctx.call<void>("fillText", title, width / 2 - header_size.width / 2,
                   height / 2 + header_size.height / 2);

    static const auto controls_hint_font = getCanvasFontString(20, "Consolas");
    ctx.set("font", controls_hint_font);
    static const std::string constrols_hint = "WSAD/arrows to control";
    const auto controls_hint_size = measureCanvasText(ctx, constrols_hint);
    ctx.call<void>(
        "fillText", constrols_hint, width / 2 - controls_hint_size.width / 2,
        overlay_vertical_margin + overlay_padding + controls_hint_size.height);

    static const auto start_hint_font = getCanvasFontString(20, "Consolas");
    ctx.set("font", start_hint_font);
    static const std::string start_hint = "space/enter to start";
    const auto start_hint_size = measureCanvasText(ctx, start_hint);
    ctx.call<void>("fillText", start_hint,
                   width / 2 - start_hint_size.width / 2,
                   height - overlay_vertical_margin - overlay_padding);
  }

  side.needs_redraw = false;
  side.needs_update_on_cube = true;
}
