#pragma once

#include "position.hpp"
#include <array>
#include <cstddef>
#include <string_view>

struct test {
  struct position position;
  std::array<size_t, 6> expected;

  test(std::string_view row);
  size_t run() const;

  static size_t run(std::string_view file);
};
