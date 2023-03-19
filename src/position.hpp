#pragma once

#include "node.hpp"
#include "side.hpp"
#include <cstddef>
#include <variant>
#include <string_view>

struct position {
  using color_t = std::variant<white_side, black_side>;

  struct node node;
  color_t color;

  position();
  position(std::string_view fen);

  size_t perft(int depth) const noexcept;
};
