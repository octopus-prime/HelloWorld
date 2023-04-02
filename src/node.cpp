#include "node.hpp"
#include "executer.hpp"
#include "generator.hpp"
#include <cstdint>

//node::node() noexcept
//    : white("12"_r),
//      black("78"_r),
//      king("e1e8"_b),
//      rook_queen("a1d1h1a8d8h8"_b),
//      bishop_queen("c1d1f1c8d8f8"_b),
//      knight("b1g1b8g8"_b),
//      pawn("27"_r),
//      castle("a1h1a8h8"_b),
//      en_passant() {}

node::node(uint64_t white, uint64_t black, uint64_t king, uint64_t rook_queen, uint64_t bishop_queen, uint64_t knight, uint64_t pawn, uint64_t castle, uint64_t en_passant) noexcept
    : white{white},
      black(black),
      king(king),
      rook_queen(rook_queen),
      bishop_queen(bishop_queen),
      knight(knight),
      pawn(pawn),
      castle(castle),
      en_passant(en_passant) {}

template <typename side>
node::node(const node& node_, const move& move, side) noexcept : node(node_) {
  execute<side>(*this, move);
}

template node::node(const node& node_, const move& move, white_side) noexcept;
template node::node(const node& node_, const move& move, black_side) noexcept;

template <typename side>
std::span<move> node::generate(std::span<move, 256> moves) const noexcept {
  return ::generate<side>(*this, moves);
}

template std::span<move> node::generate<white_side>(std::span<move, 256> moves) const noexcept;
template std::span<move> node::generate<black_side>(std::span<move, 256> moves) const noexcept;
