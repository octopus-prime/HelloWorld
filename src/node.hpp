#pragma once

#include "side.hpp"
#include <cstdint>
#include <span>
#include <string_view>
#include <type_traits>

struct move;

struct node {
  uint64_t white;
  uint64_t black;
  uint64_t king;
  uint64_t rook_queen;
  uint64_t bishop_queen;
  uint64_t knight;
  uint64_t pawn;
  uint64_t castle;
  uint64_t en_passant;

  node() noexcept;
  node(const std::string_view fen);

  template <typename side>
  node(const node& node, const move& move, side) noexcept;

  template <typename side>
  std::span<move> generate(std::span<move, 256> moves) const noexcept;
};
