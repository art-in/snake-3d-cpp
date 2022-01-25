#include <emscripten/html5.h>

#include <memory>

#include "game.hpp"

auto main() -> int {
  // allocate game object in the heap, but doesn't really have to, since
  // address-taken variables are placed in "aliased stack" in linear memory, and
  // frame of this stack doesn't get destroyed if we not finishing the function
  auto game = std::make_shared<Game>();

  // never finish executing main func to avoid game destructor
  emscripten_unwind_to_js_event_loop();
  return 0;
}
