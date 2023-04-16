#pragma once

#include "side.hpp"
#include "lookup.hpp"
#include "square_view.hpp"
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

//  node() noexcept = default;
  node(uint64_t white = 0, uint64_t black = 0, uint64_t king = 0, uint64_t rook_queen = 0, uint64_t bishop_queen = 0, uint64_t knight = 0, uint64_t pawn = 0, uint64_t castle = 0, uint64_t en_passant = 0) noexcept;

  template <typename side>
  node(const node& node, const move& move, side) noexcept;

  template <typename side>
  std::span<move> generate(std::span<move, 256> moves) const noexcept;

  template <typename side>
  uint64_t attacked(int sqaure, uint64_t color) const noexcept;

  template<typename side>
  uint64_t attacks() const noexcept;

  template <typename side>
  uint64_t check() const noexcept;
};


//template <typename side>
//uint64_t node::attacked(auto square) const noexcept {
//  using other = flip<side>;
//  auto out = lookup_kings[square] & kings<other>();
//  out |= lookup_knights[square] & knights<other>();
//  out |= lookup_rook_queen[square, white | black] & rooks_queens2<other>();
//  out |= lookup_bishop_queen[square, white | black] & bishops_queens2<other>();
//  out |= lookup_pawns<side>(square) & pawns2<other>();
//  return out;
//}

template <typename side>
inline uint64_t node::attacked(int square, uint64_t color) const noexcept {
  auto out = lookup_kings[square] & (king & color);
  out |= lookup_knights[square] & (knight & color);
  out |= lookup_rook_queen(square, white | black) & (rook_queen & color);
  out |= lookup_bishop_queen(square, white | black) & (bishop_queen & color);
  if constexpr (std::is_same_v<side, white_side>)
    out |= lookup_pawns_w[square] & (pawn & color);
  else
    out |= lookup_pawns_b[square] & (pawn & color);
  return out;
}

template <>
inline uint64_t node::check<white_side>() const noexcept {
  auto square = std::countr_zero(king & white);
  return attacked<white_side>(square, black);
}

template <>
inline uint64_t node::check<black_side>() const noexcept {
  auto square = std::countr_zero(king & black);
  return attacked<black_side>(square, white);
}

template<>
inline uint64_t node::attacks<white_side>() const noexcept {
  return ::attacks<white_side>(king, rook_queen, bishop_queen, knight, pawn, white, black);
}

template<>
inline uint64_t node::attacks<black_side>() const noexcept {
  return ::attacks<black_side>(king, rook_queen, bishop_queen, knight, pawn, black, white);
}
