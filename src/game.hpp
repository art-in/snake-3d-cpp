#pragma once

#include <emscripten/html5.h>

#include "models/GameState.hpp"

class Game {
 public:
  Game();

 private:
  GameState state;

  static auto loop(double time, void* data) -> EM_BOOL;

  void subscribe();

  static auto on_resize(int, const EmscriptenUiEvent*, void*) -> EM_BOOL;
  static auto on_keydown(int, const EmscriptenKeyboardEvent*, void*) -> EM_BOOL;
  static auto on_mousedown(int, const EmscriptenMouseEvent*, void*) -> EM_BOOL;
  static auto on_mouseup(int, const EmscriptenMouseEvent*, void*) -> EM_BOOL;
  static auto on_mousemove(int, const EmscriptenMouseEvent*, void*) -> EM_BOOL;
};
