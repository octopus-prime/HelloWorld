#include "executer.hpp"
#include "literal.hpp"

void remove(node& node, uint64_t to, white_side) noexcept {
//  if (node.black & to) {
    node.rook_queen &= ~to;
    node.bishop_queen &= ~to;
    node.knight &= ~to;
    node.pawn &= ~to;
    node.white &= ~to;
    node.castle &= ~("a8h8"_b & to);
//  }
}

void remove(node& node, uint64_t to, black_side) noexcept {
//  if (node.white & to) {
    node.rook_queen &= ~to;
    node.bishop_queen &= ~to;
    node.knight &= ~to;
    node.pawn &= ~to;
    node.white &= ~to;
    node.castle &= ~("a1h1"_b & to);
//  }
}

// move::king

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::king) noexcept {
  remove(node, to, side);
  node.king ^= from | to;
  node.white ^= from | to;
  node.castle |= "a1h1"_b;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::king) noexcept {
  remove(node, to, side);
  node.king ^= from | to;
  node.black ^= from | to;
  node.castle |= "a8h8"_b;
}

// move::king_castle_short

void execute(node& node, uint64_t, uint64_t, white_side, move::king_castle_short) noexcept {
  node.king ^= "e1g1"_b;
  node.rook_queen ^= "h1f1"_b;
  node.white ^= "e1f1g1h1"_b;
  node.castle |= "a1h1"_b;
}

void execute(node& node, uint64_t, uint64_t, black_side, move::king_castle_short) noexcept {
  node.king ^= "e8g8"_b;
  node.rook_queen ^= "h8f8"_b;
  node.black ^= "e8f8g8h8"_b;
  node.castle |= "a8h8"_b;
}

// move::king_castle_long

void execute(node& node, uint64_t, uint64_t, white_side, move::king_castle_long) noexcept {
  node.king ^= "e1c1"_b;
  node.rook_queen ^= "a1d1"_b;
  node.white ^= "a1c1d1e1"_b;
  node.castle |= "a1h1"_b;
}

void execute(node& node, uint64_t, uint64_t, black_side, move::king_castle_long) noexcept {
  node.king ^= "e8c8"_b;
  node.rook_queen ^= "a8d8"_b;
  node.black ^= "a8c8d8e8"_b;
  node.castle |= "a8h8"_b;
}

// move::queen

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::queen) noexcept {
  remove(node, to, side);
  node.rook_queen ^= from | to;
  node.bishop_queen ^= from | to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::queen) noexcept {
  remove(node, to, side);
  node.rook_queen ^= from | to;
  node.bishop_queen ^= from | to;
  node.black ^= from | to;
}

// move::rook

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::rook) noexcept {
  remove(node, to, side);
  node.rook_queen ^= from | to;
  node.white ^= from | to;
  node.castle |= "a1h1"_b & from;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::rook) noexcept {
  remove(node, to, side);
  node.rook_queen ^= from | to;
  node.black ^= from | to;
  node.castle |= "a8h8"_b & from;
}

// move::bishop

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::bishop) noexcept {
  remove(node, to, side);
  node.bishop_queen ^= from | to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::bishop) noexcept {
  remove(node, to, side);
  node.bishop_queen ^= from | to;
  node.black ^= from | to;
}

// move::knight

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::knight) noexcept {
  remove(node, to, side);
  node.knight ^= from | to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::knight) noexcept {
  remove(node, to, side);
  node.knight ^= from | to;
  node.black ^= from | to;
}

// move::knight_capture

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::knight_capture) noexcept {
  remove(node, to, side);
  node.knight ^= from | to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::knight_capture) noexcept {
  remove(node, to, side);
  node.knight ^= from | to;
  node.black ^= from | to;
}

// move::pawn_single

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_single) noexcept {
  node.pawn ^= from | to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_single) noexcept {
  node.pawn ^= from | to;
  node.black ^= from | to;
}

// move::pawn_double

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_double) noexcept {
  execute(node, from, to, side, move::pawn_single{});
  node.en_passant = to << 8;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_double) noexcept {
  execute(node, from, to, side, move::pawn_single{});
  node.en_passant = to >> 8;
}

// move::pawn_capture

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_capture) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single{});
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_capture) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single{});
}

// move::pawn_single_promote_queen

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_single_promote_queen) noexcept {
  node.pawn |= from;
  node.rook_queen ^= to;
  node.bishop_queen ^= to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_single_promote_queen) noexcept {
  node.pawn |= from;
  node.rook_queen ^= to;
  node.bishop_queen ^= to;
  node.black ^= from | to;
}

// move::pawn_capture_promote_queen

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_capture_promote_queen) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_queen{});
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_capture_promote_queen) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_queen{});
}

// move::pawn_single_promote_rook

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_single_promote_rook) noexcept {
  node.pawn |= from;
  node.rook_queen ^= to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_single_promote_rook) noexcept {
  node.pawn |= from;
  node.rook_queen ^= to;
  node.black ^= from | to;
}

// move::pawn_capture_promote_rook

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_capture_promote_rook) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_rook{});
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_capture_promote_rook) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_rook{});
}

// move::pawn_single_promote_bishop

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_single_promote_bishop) noexcept {
  node.pawn |= from;
  node.bishop_queen ^= to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_single_promote_bishop) noexcept {
  node.pawn |= from;
  node.bishop_queen ^= to;
  node.black ^= from | to;
}

// move::pawn_capture_promote_bishop

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_capture_promote_bishop) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_bishop{});
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_capture_promote_bishop) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_bishop{});
}

// move::pawn_single_promote_knight

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_single_promote_knight) noexcept {
  node.pawn |= from;
  node.knight ^= to;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_single_promote_knight) noexcept {
  node.pawn |= from;
  node.knight ^= to;
  node.black ^= from | to;
}

// move::pawn_capture_promote_knight

void execute(node& node, uint64_t from, uint64_t to, white_side side, move::pawn_capture_promote_knight) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_knight{});
}

void execute(node& node, uint64_t from, uint64_t to, black_side side, move::pawn_capture_promote_knight) noexcept {
  remove(node, to, side);
  execute(node, from, to, side, move::pawn_single_promote_knight{});
}

// move::en_passant

void execute(node& node, uint64_t from, uint64_t to, white_side, move::pawn_capture_en_passant) noexcept {
  node.pawn |= from;
  node.black |= from;
  node.white ^= from | to;
}

void execute(node& node, uint64_t from, uint64_t to, black_side, move::pawn_capture_en_passant) noexcept {
  node.pawn |= from;
  node.white |= from;
  node.black ^= from | to;
}

// --------------------------------------

template <typename side>
void execute(node& node, const move& move) noexcept {
  std::visit([&](auto&& tag) noexcept {
    node.en_passant = 0;
    execute(node, 1ull << move.from, 1ull << move.to, side{}, tag);
  }, move.tag);
}

template void execute<white_side>(node& node, const move& move) noexcept;
template void execute<black_side>(node& node, const move& move) noexcept;
