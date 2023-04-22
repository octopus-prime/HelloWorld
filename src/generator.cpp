#include "generator.hpp"
#include "lookup.hpp"
#include "literal.hpp"
#include "side.hpp"
#include "square_view.hpp"
#include <bit>
#include <ranges>

#include <iostream>

template<typename side>
std::span<move> generate(const node& node, std::span<move, 256> moves) noexcept {
  int index = 0;

  auto attacked = node.attacks<flip<side>>();
  auto checks = node.check<side>();

  auto all_pinned = 0ull;
  auto valid_r = 0ull;
  auto valid_b = 0ull;

  uint64_t valid_for_pinned[64];
  std::ranges::fill_n(valid_for_pinned, 64, ~0ull);

  auto valids = ~0ull;

  const auto generate_kings = [&](uint64_t self) noexcept {
    char from = std::countr_zero(node.king & self);
    uint64_t out = lookup_kings[from];
    for (char to : square_view(out & ~self & ~attacked))
      moves[index++] = {move::king{}, from, to};
  };

  const auto generate_castle_w = [&]() noexcept {
    auto occupied = node.white | node.black;
    constexpr int e1 = "e1"_s;
    if ((node.castle & "h1"_b) && !(occupied & "f1g1"_b) && !(attacked & "e1f1g1"_b))
      moves[index++] = {move::king_castle_short{}, e1, e1 + 2};
    if ((node.castle & "a1"_b) && !(occupied & "b1c1d1"_b) && !(attacked & "e1d1c1"_b))
      moves[index++] = {move::king_castle_long{}, e1, e1 - 2};
  };

  const auto generate_castle_b = [&]() noexcept {
    auto occupied = node.white | node.black;
    constexpr int e8 = "e8"_s;
    if ((node.castle & "h8"_b) && !(occupied & "f8g8"_b) && !(attacked & "e8f8g8"_b))
      moves[index++] = {move::king_castle_short{}, e8, e8 + 2};
    if ((node.castle & "a8"_b) && !(occupied & "b8c8d8"_b) && !(attacked & "e8d8c8"_b))
      moves[index++] = {move::king_castle_long{}, e8, e8 - 2};
  };

  const auto generate_knights = [&](uint64_t self) noexcept {
    for (char from : square_view(node.knight & self & ~all_pinned)) {
      uint64_t out = lookup_knights[from];
      for (char to : square_view(out & ~self & valids))
        moves[index++] = {move::knight{}, from, to};
    }
  };

  const auto generate_rooks_queens = [&](uint64_t self) noexcept {
    constexpr move::tag_t rook_or_queen[] = {move::rook{}, move::queen{}};
    for (char from : square_view(node.rook_queen & self)) {
      auto tag = rook_or_queen[bool((1ull << from) & node.bishop_queen)];
      uint64_t out = lookup_rook_queen(from, node.white | node.black);
      for (char to : square_view(out & ~self & valids & valid_for_pinned[from]))
        moves[index++] = {tag, from, to};
    }
  };

  const auto generate_bishops_queens = [&](uint64_t self) noexcept {
    constexpr move::tag_t bishop_or_queen[] = {move::bishop{}, move::queen{}};
    for (char from : square_view(node.bishop_queen & self)) {
      auto tag = bishop_or_queen[bool((1ull << from) & node.rook_queen)];
      uint64_t out = lookup_bishop_queen(from, node.white | node.black);
      for (char to : square_view(out & ~self & valids & valid_for_pinned[from]))
        moves[index++] = {tag, from, to};
    }
  };

  const auto generate_single_promote_w = [&](char from, char to) noexcept {
    if (to < "a8"_s) {
      moves[index++] = {move::pawn_single{}, from, to};
    } else {
      moves[index++] = {move::pawn_single_promote_queen{}, from, to};
      moves[index++] = {move::pawn_single_promote_rook{}, from, to};
      moves[index++] = {move::pawn_single_promote_bishop{}, from, to};
      moves[index++] = {move::pawn_single_promote_knight{}, from, to};
    }
  };

  const auto generate_single_promote_b = [&](char from, char to) noexcept {
    if (to > "h1"_s) {
      moves[index++] = {move::pawn_single{}, from, to};
    } else {
      moves[index++] = {move::pawn_single_promote_queen{}, from, to};
      moves[index++] = {move::pawn_single_promote_rook{}, from, to};
      moves[index++] = {move::pawn_single_promote_bishop{}, from, to};
      moves[index++] = {move::pawn_single_promote_knight{}, from, to};
    }
  };

  const auto generate_capture_promote_w = [&](char from, char to) noexcept {
    if (to < "a8"_s) {
      moves[index++] = {move::pawn_capture{}, from, to};
    } else {
      moves[index++] = {move::pawn_capture_promote_queen{}, from, to};
      moves[index++] = {move::pawn_capture_promote_rook{}, from, to};
      moves[index++] = {move::pawn_capture_promote_bishop{}, from, to};
      moves[index++] = {move::pawn_capture_promote_knight{}, from, to};
    }
  };

  const auto generate_capture_promote_b = [&](char from, char to) noexcept {
    if (to > "h1"_s) {
      moves[index++] = {move::pawn_capture{}, from, to};
    } else {
      moves[index++] = {move::pawn_capture_promote_queen{}, from, to};
      moves[index++] = {move::pawn_capture_promote_rook{}, from, to};
      moves[index++] = {move::pawn_capture_promote_bishop{}, from, to};
      moves[index++] = {move::pawn_capture_promote_knight{}, from, to};
    }
  };

  const auto generate_pawns_w2 = [&]() noexcept {
    for (char from : square_view(node.pawn & node.white)) {
      auto board = 1ull << from;
      auto front = (board << 8) & ~(node.white | node.black) & valids & valid_for_pinned[from];
      if (front != 0ull)
        generate_single_promote_w(from, from + 8);
      auto front2 = ((front & "3"_r) << 8) & ~(node.white | node.black) & valids & valid_for_pinned[from];
      if (front2 != 0ull)
        moves[index++] = {move::pawn_double{}, from, char(from + 16)};
      auto left = (board << 7) & ~"h"_f & (node.black | node.en_passant) & valids & valid_for_pinned[from];
      if (left != 0ull) {
        if (left & node.en_passant)
          moves[index++] = {move::pawn_capture_en_passant{}, from, char(from + 7)};
        else
          generate_capture_promote_w(from,  from + 7);
      }
      auto right = (board << 9) & ~"a"_f & (node.black | node.en_passant) & valids & valid_for_pinned[from];
      if (right != 0ull) {
        if (right & node.en_passant)
          moves[index++] = {move::pawn_capture_en_passant{}, from, char(from + 9)};
        else
          generate_capture_promote_w(from,  from + 9);
      }
    }
  };

  const auto generate_pawns_b2 = [&]() noexcept {
    for (char from : square_view(node.pawn & node.black)) {
      auto board = 1ull << from;
      auto front = (board >> 8) & ~(node.white | node.black) & valids & valid_for_pinned[from];
      if (front != 0ull)
        generate_single_promote_b(from, from - 8);
      auto front2 = ((front & "6"_r) >> 8) & ~(node.white | node.black) & valids & valid_for_pinned[from];
      if (front2 != 0ull)
        moves[index++] = {move::pawn_double{}, from, char(from - 16)};
      auto left = (board >> 7) & ~"a"_f & (node.white | node.en_passant) & valids & valid_for_pinned[from];
      if (left != 0ull) {
        if (left & node.en_passant)
          moves[index++] = {move::pawn_capture_en_passant{}, from, char(from - 7)};
        else
          generate_capture_promote_b(from,  from - 7);
      }
      auto right = (board >> 9) & ~"h"_f & (node.white | node.en_passant) & valids & valid_for_pinned[from];
      if (right != 0ull) {
        if (right & node.en_passant)
          moves[index++] = {move::pawn_capture_en_passant{}, from, char(from - 9)};
        else
          generate_capture_promote_b(from,  from - 9);
      }
    }
  };

  const auto generate_pawns_w = [&]() noexcept {
    const auto pawn = node.pawn & node.white;

    const auto fronta = ((~all_pinned & pawn) << 8) & ~(node.white | node.black);
    for (char to : square_view(fronta & valids))
      generate_single_promote_w(to - 8, to);

    const auto frontb = ((all_pinned & pawn) << 8) & ~(node.white | node.black);
    for (char to : square_view(frontb & valids & valid_r))
      generate_single_promote_w(to - 8, to);

    const auto front2 = ((fronta >> 8) & "2"_r) << 16;
    for (char to : square_view(front2 & ~(node.white | node.black) & valids))
      moves[index++] = {move::pawn_double{}, char(to - 16), to};

    const auto front3 = ((frontb >> 8) & "2"_r) << 16;
    for (char to : square_view(front3 & ~(node.white | node.black) & valids & valid_r))
      moves[index++] = {move::pawn_double{}, char(to - 16), to};

    const auto lefta = ((~all_pinned & pawn) << 7) & ~"h"_f;
    for (char to : square_view(lefta & node.black & valids))
      generate_capture_promote_w(to - 7, to);

    const auto leftb = ((all_pinned & pawn) << 7) & ~"h"_f;
    for (char to : square_view(leftb & node.black & valids & valid_b))
      generate_capture_promote_w(to - 7, to);

    const auto righta = ((~all_pinned & pawn) << 9) & ~"a"_f;
    for (char to : square_view(righta & node.black & valids))
      generate_capture_promote_w(to - 9, to);

    const auto rightb = ((all_pinned & pawn) << 9) & ~"a"_f;
    for (char to : square_view(rightb & node.black & valids & valid_b))
      generate_capture_promote_w(to - 9, to);

    auto ep = node.en_passant & (valids | ((checks & node.pawn & node.black & node.en_passant >> 8) << 8));
    const auto en_passant = (((ep >> 7) & ~"a"_f) | ((ep >> 9) & ~"h"_f)) & node.pawn & node.white & ~all_pinned;
    for (char from : square_view(en_passant))
      moves[index++] = {move::pawn_capture_en_passant{}, from, (char) std::countr_zero(node.en_passant)};
  };

  const auto generate_pawns_b = [&]() noexcept {
    const auto pawn = node.pawn & node.black;

    const auto fronta = ((~all_pinned & pawn) >> 8) & ~(node.white | node.black);
    for (char to : square_view(fronta & valids))
      generate_single_promote_b(to + 8, to);

    const auto frontb = ((all_pinned & pawn) >> 8) & ~(node.white | node.black);
    for (char to : square_view(frontb & valids & valid_r))
      generate_single_promote_b(to + 8, to);

    const auto front2 = ((fronta << 8) & "7"_r) >> 16;
    for (char to : square_view(front2 & ~(node.white | node.black) & valids))
      moves[index++] = {move::pawn_double{}, char(to + 16), to};

    const auto front3 = ((frontb << 8) & "7"_r) >> 16;
    for (char to : square_view(front3 & ~(node.white | node.black) & valids & valid_r))
      moves[index++] = {move::pawn_double{}, char(to + 16), to};

    const auto lefta = ((~all_pinned & pawn) >> 9) & ~"h"_f;
    for (char to : square_view(lefta & node.white & valids))
      generate_capture_promote_b(to + 9, to);

    const auto leftb = ((all_pinned & pawn) >> 9) & ~"h"_f;
    for (char to : square_view(leftb & node.white & valids & valid_b))
      generate_capture_promote_b(to + 9, to);

    const auto righta = ((~all_pinned & pawn) >> 7) & ~"a"_f;
    for (char to : square_view(righta & node.white & valids))
      generate_capture_promote_b(to + 7, to);

    const auto rightb = ((all_pinned & pawn) >> 7) & ~"a"_f;
    for (char to : square_view(rightb & node.white & valids & valid_b))
      generate_capture_promote_b(to + 7, to);

    auto ep = node.en_passant & (valids | ((checks & node.pawn & node.white & node.en_passant << 8) >> 8));
    const auto en_passant = (((ep << 7) & ~"h"_f) | ((ep << 9) & ~"a"_f)) & node.pawn & node.black & ~all_pinned;
    for (char from : square_view(en_passant))
      moves[index++] = {move::pawn_capture_en_passant{}, from, (char) std::countr_zero(node.en_passant)};
  };

  const auto generate_pinned = [&](uint64_t self, uint64_t other) noexcept {
    auto king = std::countr_zero(node.king & self);
    auto ksr = lookup_rook_queen(king, node.white | node.black) & self;
    auto ksb = lookup_bishop_queen(king, node.white | node.black) & self;
    auto bcr = lookup_rook_queen(king, 0ull) & node.rook_queen & other;
    auto bcb = lookup_bishop_queen(king, 0ull) & node.bishop_queen & other;
    for (auto square : square_view{bcr}) {
      auto rsk = lookup_rook_queen(square, node.white | node.black) & self;
      auto pinned = (ksr & rsk);
      all_pinned |= pinned;
      if (pinned) {
        valid_for_pinned[std::countr_zero(pinned)] = lookup_lines[king][square];
        valid_r |= lookup_lines[king][square];
      }
    }
    for (auto square : square_view{bcb}) {
      auto bsk = lookup_bishop_queen(square, node.white | node.black) & self;
      auto pinned = (ksb & bsk);
      all_pinned |= pinned;
      if (pinned) {
        valid_for_pinned[std::countr_zero(pinned)] = lookup_lines[king][square];
        valid_b |= lookup_lines[king][square];
      }
    }
//    std::cout << "king = " << king << std::endl;
//    std::cout << "ksr = " << ksr << std::endl;
//    std::cout << "ksb = " << ksb << std::endl;
//    std::cout << "bcr = " << bcr << std::endl;
//    std::cout << "bcb = " << bcb << std::endl;
//    std::cout << "pinned = " << all_pinned << std::endl;
//    std::cout << "valid_r = " << valid_r << std::endl;
//    std::cout << "valid_b = " << valid_b << std::endl;
  };

  if constexpr (std::is_same_v<side, white_side>) {
    generate_kings(node.white);
    if (std::popcount(checks) > 1)
        return moves.subspan(0, index);
    if (std::popcount(checks) == 1) {
      auto to = std::countr_zero(checks);
      auto from = std::countr_zero(node.king & node.white);
      valids = lookup_lines[from][to];
    }
    else
      generate_castle_w();
    generate_pinned(node.white, node.black);
    generate_knights(node.white);
    generate_rooks_queens(node.white);
    generate_bishops_queens(node.white);
    generate_pawns_w();
  } else {
    generate_kings(node.black);
    if (std::popcount(checks) > 1)
        return moves.subspan(0, index);
    if (std::popcount(checks) == 1) {
      auto to = std::countr_zero(checks);
      auto from = std::countr_zero(node.king & node.black);
      valids = lookup_lines[from][to];
    }
    else
      generate_castle_b();
    generate_pinned(node.black, node.white);
    generate_knights(node.black);
    generate_rooks_queens(node.black);
    generate_bishops_queens(node.black);
    generate_pawns_b();
  }

  return moves.subspan(0, index);
}

template std::span<move> generate<white_side>(const node& node, std::span<move, 256> moves) noexcept;
template std::span<move> generate<black_side>(const node& node, std::span<move, 256> moves) noexcept;
