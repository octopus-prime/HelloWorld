#pragma once

#include "square.hpp"
#include <bit>
#include <numeric>
#include <ranges>
#include <vector>
#include <array>
#include <immintrin.h> // _pext

#include "literal.hpp"
#include <algorithm>

class bitboard {
  using storage_t = std::uint64_t;

  class iterator;
  class lookup;
  
  storage_t bits;
  
public:
  constexpr bitboard(storage_t bits = 0ull) noexcept
      : bits{bits} {}

  constexpr explicit bitboard(square square) noexcept
      : bitboard{1ull << square} {}

  constexpr bitboard(std::string_view string)
      : bitboard{parse(string)} {}

  constexpr operator storage_t() const noexcept {
    return bits;
  }

  constexpr bool empty() const noexcept {
    return bits == 0ull;
  }

  constexpr auto count() const noexcept {
    return std::popcount(bits);
  }
  
  constexpr square find() const noexcept {
    return std::countr_zero(bits);
  }

  constexpr square pop() noexcept {
    square square = find();
    flip(square);
    return square;
  }

  constexpr void set(square square) noexcept {
    bits |= square;
  }

  constexpr void set(bitboard squares) noexcept {
    bits |= squares;
  }

  constexpr void reset(square square) noexcept {
    bits &= ~square;
  }

  constexpr void reset(bitboard squares) noexcept {
    bits &= ~squares;
  }

  constexpr void flip(square square) noexcept {
    bits ^= square;
  }

  constexpr void flip(bitboard squares) noexcept {
    bits ^= squares;
  }

  constexpr iterator begin() const noexcept;

  constexpr iterator end() const noexcept;

  static constexpr bitboard parse(std::string_view string) {
    auto view = string | std::views::chunk(2);
    return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [](auto&& chunk) {
      return bitboard{square{std::string_view{chunk.data(), chunk.size()}}};
    });
  }
  
  static constexpr bitboard expand_king(square square) noexcept {
//    return lookup_kings[square];
    return expand_kings(bitboard{square});
  }

  static constexpr bitboard expand_kings(bitboard squares) noexcept {
    constexpr int shifts[] = {1, 8, 7, 9};
    constexpr uint64_t masks_left[] = {~"a"_f, ~""_f, ~"h"_f, ~"a"_f};
    constexpr uint64_t masks_right[] = {~"h"_f, ~""_f, ~"a"_f, ~"h"_f};
    return expand_leaper(squares, std::views::zip(shifts, masks_left, masks_right));
  }

  static constexpr bitboard expand_knight(square square) noexcept {
    return lookup_knights[square];
  }

  static constexpr bitboard expand_knights(bitboard squares) noexcept {
    constexpr int shifts[] = {10, 17, 15, 6};
    constexpr uint64_t masks_left[] = {~"ab"_f, ~"a"_f, ~"h"_f, ~"gh"_f};
    constexpr uint64_t masks_right[] = {~"gh"_f, ~"h"_f, ~"a"_f, ~"ab"_f};
    return expand_leaper(squares, std::views::zip(shifts, masks_left, masks_right));
  }

  static constexpr std::array<bitboard, 64> expand_leaper(auto&& expand) noexcept;
  
private:
  static constexpr bitboard expand_leaper(bitboard in, auto&& view) noexcept {
    auto expand = [in](auto shift, auto mask_left, auto mask_right) noexcept {
      bitboard left = (in << shift) & mask_left;
      bitboard right = (in >> shift) & mask_right;
      return left | right;
    };
    return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [expand](auto&& tuple) noexcept {
      return std::apply(expand, tuple);
    });
  }

  static constinit std::array<bitboard, 64> lookup_kings;
  static constinit std::array<bitboard, 64> lookup_knights;
  static constinit lookup lookup_rook_queen;
};

class bitboard::iterator {
  bitboard bits;

public:
  constexpr iterator(bitboard bits) noexcept
      : bits(bits) {}

  constexpr void operator++() const noexcept {}

  constexpr bool operator!=(iterator) const noexcept {
    return !bits.empty();
  }

  constexpr square operator*() noexcept {
    return bits.pop();
  }
};

constexpr bitboard::iterator bitboard::begin() const noexcept {
  return {bits};
}

constexpr bitboard::iterator bitboard::end() const noexcept {
  return {0ull};
}

class bitboard::lookup {
  struct block {
    bitboard mask;
    std::vector<bitboard> data;
  };

  std::array<block, 64> blocks;

public:
  constexpr bitboard operator()(auto square, bitboard occupied) const noexcept {
    const auto& block = blocks[square];
    const auto index = _pext_u64(occupied, block.mask);
    return block.data[index];
  }
};

constexpr bitboard operator""_b2(const char* data, size_t length) {
  return std::string_view{data, length};
}

std::ostream& operator<<(std::ostream& stream, bitboard bitboard);
