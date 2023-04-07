#include "bitboard.hpp"
#include "square.hpp"
#include <span>

void foo() {
  square s{3u, 5u};
  constexpr square s2{"b3"};
  bitboard b{square(13u)};
  b = b << 3;
  b.set(b);
  b.set(square{13u, 1u});
  b.set(bitboard{123ull});
  for (auto square : b) {
    square.file();
    square.rank();
  }
}

class move2 {
  square _from;
  square _to;

public:
  constexpr move2(square from, square to)
      : _from{from}, _to{to} {}

  constexpr square from() const noexcept {
    return _from;
  }

  constexpr square to() const noexcept {
    return _to;
  }
};

struct node2 {
  bitboard white;
  bitboard black;
  bitboard king;
  bitboard rook_queen;
  bitboard bishop_queen;
  bitboard knight;
  bitboard pawn;
  bitboard castle;
  bitboard en_passant;

  //  node2() noexcept = default;
  node2(uint64_t white, uint64_t black, uint64_t king, uint64_t rook_queen, uint64_t bishop_queen, uint64_t knight, uint64_t pawn, uint64_t castle, uint64_t en_passant) noexcept;
  node2(const std::string_view fen);

  template <typename side>
  node2(const node2& node, const move2& move, side) noexcept {
  }

  template <typename side>
  std::span<move2> generate(std::span<move2, 256> moves) const noexcept;

  template <typename side>
  uint64_t attacked(int sqaure, uint64_t color) const noexcept;

  template <typename side>
  uint64_t check() const noexcept;

private:
  constexpr void remove(square to) noexcept {
    rook_queen.reset(to);
    bishop_queen.reset(to);
    knight.reset(to);
    pawn.reset(to);
//    constexpr auto x = bitboard::expand_king(to);
//    constexpr auto y = bitboard::expand_knight(to);
  }
};
