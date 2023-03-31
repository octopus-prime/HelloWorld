#pragma once

#include "position.hpp"
#include <array>
#include <cstddef>
#include <string_view>
#include <expected>

struct test {
  struct position position;
  std::array<size_t, 6> expected;

  test(std::string_view row);
  std::expected<size_t, std::string> run(int depth) const noexcept;

  static size_t run(std::string_view file, int depth);
};
