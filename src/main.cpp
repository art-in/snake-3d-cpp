#include <emscripten/html5.h>

#include "game.hpp"

auto main() -> int {
  Game game;

  // never finish executing main func to avoid game destructor
  emscripten_unwind_to_js_event_loop();
  return 0;
}
