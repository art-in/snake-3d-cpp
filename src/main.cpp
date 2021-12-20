#include <emscripten/html5.h>

#include <functional>
#include <iostream>

auto loop_(double time, void* data)  // NOLINT(misc-unused-parameters)
    -> EM_BOOL {
  std::cout << "Hello world" << std::endl;
  return EM_TRUE;
};

auto main() -> int {
  // start requestAnimationFrame loop
  emscripten_request_animation_frame_loop(&loop_, nullptr);

  // never finish executing main func, as otherwise the scene will be destroyed
  emscripten_unwind_to_js_event_loop();
  return 0;
}
