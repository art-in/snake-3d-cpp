#pragma once

#include <emscripten.h>
#include <emscripten/val.h>

#include <sstream>
#include <string>

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage) no constexpr alternative
#define ASSERT(condition)                                                    \
  {                                                                          \
    std::ostringstream assert_os;                                            \
    assert_os << #condition << " is false, " << __FILE__ << ":" << __LINE__; \
    assert(condition, assert_os.str());                                      \
  }

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASSERT_NOT_EMPTY(condition)                                          \
  {                                                                          \
    std::ostringstream assert_os;                                            \
    assert_os << #condition << " is empty, " << __FILE__ << ":" << __LINE__; \
    assertNotEmpty(condition, assert_os.str());                              \
  }

void assert(bool condition, const std::string& error_message);
void assertNotEmpty(const emscripten::val& v, const std::string& error_message);