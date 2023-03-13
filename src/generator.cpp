#include "generator.hpp"
#include "literal.hpp"
#include <ranges>

constexpr uint64_t expand_kings(uint64_t in) noexcept {
  constexpr int shifts[] = {1, 8, 7, 9};
  constexpr uint64_t masks_left[] = {~"a"_f, ~""_f, ~"h"_f, ~"a"_f};
  constexpr uint64_t masks_right[] = {~"h"_f, ~""_f, ~"a"_f, ~"h"_f};
  const auto view = std::views::zip(shifts, masks_left, masks_right);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in](auto&& tuple) noexcept {
    const uint64_t left = (in << std::get<0>(tuple)) & std::get<1>(tuple);
    const uint64_t right = (in >> std::get<0>(tuple)) & std::get<2>(tuple);
    return left | right;
  });
}

static constexpr std::array<uint64_t, 64> lookup_kings = []() noexcept {
  const auto view = std::views::iota(0) | std::views::transform([](auto index){
    return expand_kings(1ull << index);
  });
  std::array<uint64_t, 64> data;
  std::copy_n(view.begin(), 64, data.begin());
  return data;
}();

constexpr uint64_t expand_knights(uint64_t in) noexcept {
  constexpr int shifts[] = {10, 17, 15, 6};
  constexpr uint64_t masks_left[] = {~"ab"_f, ~"a"_f, ~"h"_f, ~"gh"_f};
  constexpr uint64_t masks_right[] = {~"gh"_f, ~"h"_f, ~"a"_f, ~"ab"_f};
  const auto view = std::views::zip(shifts, masks_left, masks_right);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in](auto&& tuple) noexcept {
    const uint64_t left = (in << std::get<0>(tuple)) & std::get<1>(tuple);
    const uint64_t right = (in >> std::get<0>(tuple)) & std::get<2>(tuple);
    return left | right;
  });
}

static constexpr std::array<uint64_t, 64> lookup_knights = []() noexcept {
  std::array<uint64_t, 64> v;
  for (int i = 0; i < 64; ++i)
    v[i] = expand_knights(1ull << i);
  return v;
}();

class square_view
{
public:
  class iterator
	{
  public:
    constexpr iterator(const uint64_t board) noexcept : _board(board) {}

    constexpr void operator++() const noexcept {}

    constexpr bool operator!=(const iterator) const noexcept {
      return _board != 0UL;
    }

    auto operator*() noexcept {
      const auto square = std::countr_zero(_board);
      _board ^= 1ull << square;
      return square;
    }

  private:
    uint64_t _board;
  };

  constexpr square_view(const uint64_t board) noexcept : _board(board)	{}

  constexpr iterator begin() const noexcept {
    return iterator(_board);
  }

  constexpr iterator end() const noexcept{
    return iterator(0ull);
  }

private:
  uint64_t _board;
};

//struct king_generator {
//  template<typename side>
//  int generate(const node& node, std::span<move> moves) const noexcept;
//};
//
//struct king_castle_short_generator {
//  template<typename side>
//  int generate(const node& node, std::span<move> moves) const noexcept;
//};
//
//template<> int king_castle_short_generator::generate<white_side>(const node &node, std::span<move> moves) const noexcept {
//  //  if ((node.castle & "g1"_b) && !(node.occupied() & "f1g1"_b) &&
//  //      !attacked<side>(e1) && !attacked<side>(e1 + 1) &&
//  //      !attacked<side>(e1 + 2))
//  //    *end++ = move_t{'k', (char)e1, (char)e1 + 2};
//
//  int index = 0;
//  moves[index++] = {"e1"_s, "g1"_s, king_castle_short_executer<white_side>{}};
//
//  return index;
//}
//
//template<> int king_castle_short_generator::generate<black_side>(const node &node, std::span<move> moves) const noexcept {
//  //  if ((node.castle & "g1"_b) && !(node.occupied() & "f1g1"_b) &&
//  //      !attacked<side>(e1) && !attacked<side>(e1 + 1) &&
//  //      !attacked<side>(e1 + 2))
//  //    *end++ = move_t{'k', (char)e1, (char)e1 + 2};
//
//  int index = 0;
//  moves[index++] = {"e8"_s, "g8"_s, king_castle_short_executer<black_side>{}};
//
//  return index;
//}
//
//struct king_castle_long_generator {
//  template<typename side>
//  int generate(const node& node, std::span<move> moves) const noexcept;
//};

template<typename side>
std::span<move> generate(const node& node, std::span<move, 256> moves) noexcept {
  int index = 0;
//  index += king_generator{}.generate<side>(node, moves.subspan(index));
//  index += king_castle_short_generator{}.generate<side>(node, moves.subspan(index));
//  index += king_castle_long_generator{}.generate<side>(node, moves.subspan(index));

//  moves[index++] = {move::king{}, 13, 17};
//  moves[index++] = {move::castle_short{}};
//  moves[index++] = {move::castle_long{}};
//  moves[index++] = {move::queen{}, 13, 17};
//  moves[index++] = {move::rook{}, 13, 17};
//  moves[index++] = {move::bishop{}, 13, 17};
//  moves[index++] = {move::knight{}, 13, 17};
//  moves[index++] = {move::pawn{}, 13, 17};
//  moves[index++] = {move::promote_queen{}, 13, 17};
//  moves[index++] = {move::promote_rook{}, 13, 17};
//  moves[index++] = {move::promote_bishop{}, 13, 17};
//  moves[index++] = {move::promote_knight{}, 13, 17};
//  moves[index++] = {move::en_passant{}, 13, 17};

  const auto generate_kings = [&](uint64_t self) noexcept {
    auto from = std::countr_zero(node.king & self);
    uint64_t out = lookup_kings[from];
    for (auto to : square_view(out & ~self))
      moves[index++] = {move::king{}, (char)from, (char)to};
  };

  const auto generate_knights = [&](uint64_t self) noexcept {
    for (auto from : square_view(node.knight & self)) {
      uint64_t out = lookup_knights[from];
      for (auto to : square_view(out & ~self))
        moves[index++] = {move::knight{}, (char)from, (char)to};
    }
  };

  if constexpr (std::is_same_v<side, white_side>) {
    generate_kings(node.white);
    generate_knights(node.white);
  } else {
    generate_kings(node.black);
    generate_knights(node.black);
  }

  constexpr auto foo = lookup_kings[13];

  return moves.subspan(0, index);
}

template std::span<move> generate<white_side>(const node& node, std::span<move, 256> moves) noexcept;
template std::span<move> generate<black_side>(const node& node, std::span<move, 256> moves) noexcept;
