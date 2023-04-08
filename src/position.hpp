#pragma once

#include "node.hpp"
#include "side.hpp"
#include <cstddef>
#include <variant>
#include <string_view>
#include <execution>

struct position {
  using color_t = std::variant<white_side, black_side>;

  struct node node;
  color_t color;

  position();
  position(std::string_view fen);

  size_t perft(int depth) const noexcept;
  std::tuple<size_t, size_t> perft2(int depth) const noexcept;
  
  size_t perftp(int depth) const noexcept;

  void printt(int depth) const noexcept;
};
