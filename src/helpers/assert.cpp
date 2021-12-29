#include "assert.hpp"

void assert(bool condition, const std::string& error_message) {
  if (!condition) {
    emscripten_throw_string(error_message.c_str());
  }
}

void assertNotEmpty(const emscripten::val& v,
                    const std::string& error_message) {
  if (v.isNull() || v.isUndefined()) {
    emscripten_throw_string(error_message.c_str());
  }
}
