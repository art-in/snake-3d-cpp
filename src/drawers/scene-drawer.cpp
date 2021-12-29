#include "scene-drawer.hpp"

#include "cube-drawer/cube-drawer.hpp"
#include "cube-side-drawer.hpp"

void initSceneDrawer(GameState* state, emscripten::val canvas) {
  state->scene.canvas = canvas;

  for (auto& [side_type, side] : state->scene.cube.sides) {
    initCubeSideDrawer(state, side_type);
  }

  initCubeDrawer(state);
}

void drawSceneLoop(GameState* state) {
  for (auto& [side_type, side] : state->scene.cube.sides) {
    drawCubeSideLoop(state, side_type);
  }

  drawCubeLoop(state);
}
