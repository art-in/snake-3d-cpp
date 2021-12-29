#include "game.hpp"

#include <optional>

#include "actions/control-actions.hpp"
#include "actions/game-actions.hpp"
#include "drawers/scene-drawer.hpp"
#include "helpers/canvas.hpp"
#include "models/Size.hpp"

Game::Game() {
  auto document = emscripten::val::global("document");
  auto canvas =
      document.call<emscripten::val, std::string>("querySelector", "canvas");

  initGameState(&state);
  initSceneDrawer(&state, canvas);

  on_resize(0, nullptr, nullptr);
  subscribe();

  // start game loop
  emscripten_request_animation_frame_loop(&Game::loop, this);
}

auto Game::loop([[maybe_unused]] double time, void* data) -> EM_BOOL {
  auto& game = *static_cast<Game*>(data);

  updateGameStateLoop(&game.state);
  drawSceneLoop(&game.state);

  return EM_TRUE;
};

void Game::subscribe() {
  const auto* window =
      EMSCRIPTEN_EVENT_TARGET_WINDOW;  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

  emscripten_set_resize_callback(window, &state, false, &on_resize);
  emscripten_set_keydown_callback(window, &state, false, &on_keydown);
  emscripten_set_mousedown_callback(window, &state, false, &on_mousedown);
  emscripten_set_mouseup_callback(window, &state, false, &on_mouseup);
  emscripten_set_mousemove_callback(window, &state, false, &on_mousemove);
}

auto Game::on_resize([[maybe_unused]] int event_type,
                     [[maybe_unused]] const EmscriptenUiEvent* event,
                     [[maybe_unused]] void* data) -> EM_BOOL {
  auto document = emscripten::val::global("document");
  auto body = document["body"];
  auto canvas =
      document.call<emscripten::val, std::string>("querySelector", "canvas");

  const Size window_size{.width = body["clientWidth"].as<double>(),
                         .height = body["clientHeight"].as<double>()};

  resizeCanvas(canvas, window_size,
               emscripten::val::global("devicePixelRatio").as<double>());

  return EM_FALSE;
}

auto Game::on_keydown([[maybe_unused]] int event_type,
                      [[maybe_unused]] const EmscriptenKeyboardEvent* event,
                      void* data) -> EM_BOOL {
  auto* state = static_cast<GameState*>(data);
  onKeyDown(state, std::string{event->code});  // NOLINT(hicpp-no-array-decay)
  return EM_FALSE;
}

auto Game::on_mousedown([[maybe_unused]] int event_type,
                        [[maybe_unused]] const EmscriptenMouseEvent* event,
                        void* data) -> EM_BOOL {
  auto* state = static_cast<GameState*>(data);
  onMouseDown(state);
  return EM_FALSE;
}

auto Game::on_mouseup([[maybe_unused]] int event_type,
                      [[maybe_unused]] const EmscriptenMouseEvent* event,
                      void* data) -> EM_BOOL {
  auto* state = static_cast<GameState*>(data);
  onMouseUp(state);
  return EM_FALSE;
}

auto Game::on_mousemove([[maybe_unused]] int event_type,
                        const EmscriptenMouseEvent* event, void* data)
    -> EM_BOOL {
  auto* state = static_cast<GameState*>(data);
  onMouseMove(state, {.x = static_cast<double>(event->clientX),
                      .y = static_cast<double>(event->clientY)});
  return EM_FALSE;
}