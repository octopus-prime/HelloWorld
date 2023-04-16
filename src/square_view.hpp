#pragma once

#include <bit>
#include <cstdint>

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
