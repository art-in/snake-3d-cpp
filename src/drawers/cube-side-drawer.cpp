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
  const auto& grid = state->scene.cube.grid;

  const auto cell_width = width / grid.cols_count;
  const auto cell_height = height / grid.rows_count;

  for (int i = 1; i < grid.cols_count; ++i) {
    const auto x = i * cell_width;
    ctx.call<void>("moveTo", x, 0);
    ctx.call<void>("lineTo", x, height);
  }

  for (int i = 1; i < grid.rows_count; ++i) {
    const auto y = i * cell_height;
    ctx.call<void>("moveTo", 0, y);
    ctx.call<void>("lineTo", width, y);
  }

  ctx.set("lineWidth", 1);
  ctx.call<void>("stroke");

  // draw snake
  ctx.set("fillStyle", "red");
  for (const auto& part : state->snake.parts) {
    if (part.side == side_type) {
      ctx.call<void>("fillRect", part.col * cell_width,
                     height - part.row * cell_height - cell_height, cell_width,
                     cell_height);
    }
  }

  // draw apples
  ctx.set("fillStyle", "green");
  for (const auto& apple : state->apples) {
    if (apple.side == side_type) {
      ctx.call<void>("fillRect", apple.col * cell_width,
                     height - apple.row * cell_height - cell_height, cell_width,
                     cell_height);
    }
  }

  // draw stones
  ctx.set("fillStyle", "black");
  for (const auto& stone : state->stones) {
    if (stone.side == side_type) {
      ctx.call<void>("fillRect", stone.col * cell_width,
                     height - stone.row * cell_height - cell_height, cell_width,
                     cell_height);
    }
  }

  // draw status overlay
  if (state->status != EGameStatus::InGame) {
    static const auto OVERLAY_HEIGHT = 200;
    static const auto OVERLAY_WIDTH = 400;
    static const auto OVERLAY_PADDING = 30;

    const auto overlay_horizontal_margin = (width - OVERLAY_WIDTH) / 2;
    const auto overlay_vertical_margin = (height - OVERLAY_HEIGHT) / 2;

    ctx.set("globalAlpha", 0.7);
    ctx.set("fillStyle", "white");
    ctx.call<void>("fillRect", overlay_horizontal_margin,
                   overlay_vertical_margin, OVERLAY_WIDTH, OVERLAY_HEIGHT);

    ctx.set("lineWidth", 3);
    ctx.set("strokeStyle", "black");
    ctx.call<void>("strokeRect", overlay_horizontal_margin,
                   overlay_vertical_margin, OVERLAY_WIDTH, OVERLAY_HEIGHT);

    // title
    ctx.set("fillStyle", "black");
    static const auto title_font =
        getCanvasFontString(70, "Consolas", "px", "bold");
    ctx.set("font", title_font);
    const std::string title = state->status == EGameStatus::Paused ? "PAUSED"
                              : state->status == EGameStatus::Win  ? "WIN"
                              : state->status == EGameStatus::Fail ? "FAIL"
                                                                   : "SNAKE 3D";

    const auto title_size = measureCanvasText(ctx, title);
    ctx.call<void>("fillText", title, width / 2 - title_size.width / 2,
                   height / 2 + title_size.height / 2);

    // controls hint
    static const auto controls_hint_font = getCanvasFontString(20, "Consolas");
    ctx.set("font", controls_hint_font);
    static const std::string constrols_hint = "WSAD/arrows to control";
    const auto controls_hint_size = measureCanvasText(ctx, constrols_hint);
    ctx.call<void>(
        "fillText", constrols_hint, width / 2 - controls_hint_size.width / 2,
        overlay_vertical_margin + OVERLAY_PADDING + controls_hint_size.height);

    // start hint
    static const auto start_hint_font = getCanvasFontString(20, "Consolas");
    ctx.set("font", start_hint_font);
    static const std::string start_hint = "space/enter to start";
    const auto start_hint_size = measureCanvasText(ctx, start_hint);
    ctx.call<void>("fillText", start_hint,
                   width / 2 - start_hint_size.width / 2,
                   height - overlay_vertical_margin - OVERLAY_PADDING);
  }

  side.needs_redraw = false;
  side.needs_update_on_cube = true;
}
