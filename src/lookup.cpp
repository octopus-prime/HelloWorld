#include "lookup.hpp"
#include "literal.hpp"
#include "side.hpp"
#include <ranges>

constexpr uint64_t expand(uint64_t in, auto&& view) noexcept {
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in](auto&& tuple) noexcept {
    const uint64_t left = (in << std::get<0>(tuple)) & std::get<1>(tuple);
    const uint64_t right = (in >> std::get<0>(tuple)) & std::get<2>(tuple);
    return left | right;
  });
}

constexpr uint64_t expand_kings(uint64_t in) noexcept {
  constexpr int shifts[] = {1, 8, 7, 9};
  constexpr uint64_t masks_left[] = {~"a"_f, ~""_f, ~"h"_f, ~"a"_f};
  constexpr uint64_t masks_right[] = {~"h"_f, ~""_f, ~"a"_f, ~"h"_f};
  return expand(in, std::views::zip(shifts, masks_left, masks_right));
}

constexpr uint64_t expand_knights(uint64_t in) noexcept {
  constexpr int shifts[] = {10, 17, 15, 6};
  constexpr uint64_t masks_left[] = {~"ab"_f, ~"a"_f, ~"h"_f, ~"gh"_f};
  constexpr uint64_t masks_right[] = {~"gh"_f, ~"h"_f, ~"a"_f, ~"ab"_f};
  return expand(in, std::views::zip(shifts, masks_left, masks_right));
}

constexpr uint64_t expand_pawns(uint64_t in, white_side) noexcept {
  constexpr int shifts[] = {7, 9};
  constexpr uint64_t masks[] = {~"h"_f, ~"a"_f};
  const auto view = std::views::zip(shifts, masks);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in](auto&& tuple) noexcept {
    return (in << std::get<0>(tuple)) & std::get<1>(tuple);
  });
}

constexpr uint64_t expand_pawns(uint64_t in, black_side) noexcept {
  constexpr int shifts[] = {7, 9};
  constexpr uint64_t masks[] = {~"a"_f, ~"h"_f};
  const auto view = std::views::zip(shifts, masks);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in](auto&& tuple) noexcept {
    return (in >> std::get<0>(tuple)) & std::get<1>(tuple);
  });
}

constexpr uint64_t expand(uint64_t in, uint64_t empty, auto&& view) noexcept {
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in, empty](auto&& tuple) noexcept {
    uint64_t left(in);
    uint64_t right(in);
    uint64_t board(empty & std::get<1>(tuple));

    left |= board & (left << std::get<0>(tuple));
    board &= (board << std::get<0>(tuple));
    left |= board & (left << (std::get<0>(tuple) * 2));
    board &= (board << (std::get<0>(tuple) * 2));
    left |= board & (left << (std::get<0>(tuple) * 4));
    left = (left << std::get<0>(tuple)) & std::get<1>(tuple);

    board = empty & std::get<2>(tuple);

    right |= board & (right >> std::get<0>(tuple));
    board &= (board >> std::get<0>(tuple));
    right |= board & (right >> (std::get<0>(tuple) * 2));
    board &= (board >> (std::get<0>(tuple) * 2));
    right |= board & (right >> (std::get<0>(tuple) * 4));
    right = (right >> std::get<0>(tuple)) & std::get<2>(tuple);

    return left | right;
  });
}

constexpr uint64_t expand_rooks(uint64_t in, uint64_t empty) noexcept {
  constexpr int shifts[] = {1, 8};
  constexpr uint64_t masks_left[] = {~"a"_f, ~""_f};
  constexpr uint64_t masks_right[] = {~"h"_f, ~""_f};
  return expand(in, empty, std::views::zip(shifts, masks_left, masks_right));
}

constexpr uint64_t expand_bishops(uint64_t in, uint64_t empty) noexcept {
  constexpr int shifts[] = {7, 9};
  constexpr uint64_t masks_left[] = {~"h"_f, ~"a"_f};
  constexpr uint64_t masks_right[] = {~"a"_f, ~"h"_f};
  return expand(in, empty, std::views::zip(shifts, masks_left, masks_right));
}

constexpr uint64_t expand_sliders(uint64_t in, uint64_t empty) noexcept {
  constexpr int shifts[] = {1, 8, 7, 9};
  constexpr uint64_t masks_left[] = {~"a"_f, ~""_f, ~"h"_f, ~"a"_f};
  constexpr uint64_t masks_right[] = {~"h"_f, ~""_f, ~"a"_f, ~"h"_f};
  const auto view = std::views::zip(shifts, masks_left, masks_right);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [in, empty](auto&& tuple) noexcept {
    uint64_t left(in);
    uint64_t right(in);
    uint64_t board(empty & std::get<1>(tuple));

    left |= board & (left << std::get<0>(tuple));
    board &= (board << std::get<0>(tuple));
    left |= board & (left << (std::get<0>(tuple) * 2));
    board &= (board << (std::get<0>(tuple) * 2));
    left |= board & (left << (std::get<0>(tuple) * 4));
    left = (left << std::get<0>(tuple)) & std::get<1>(tuple);

    board = empty & std::get<2>(tuple);

    right |= board & (right >> std::get<0>(tuple));
    board &= (board >> std::get<0>(tuple));
    right |= board & (right >> (std::get<0>(tuple) * 2));
    board &= (board >> (std::get<0>(tuple) * 2));
    right |= board & (right >> (std::get<0>(tuple) * 4));
    right = (right >> std::get<0>(tuple)) & std::get<2>(tuple);

    return left | right;
  });
}

constexpr uint64_t blockers_permutation(uint64_t iteration, uint64_t mask) noexcept {
  uint64_t blockers = 0ull;
  while (iteration != 0ull) {
    if ((iteration & 1ull) != 0ull) {
      int shift = std::countr_zero(mask);
      blockers |= (1ull << shift);
    }
    iteration >>= 1;
    mask &= (mask - 1ull);
  }
  return blockers;
}

const std::array<uint64_t, 64> lookup_kings = []() noexcept {
  const auto view = std::views::iota(0) | std::views::transform([](auto index) noexcept {
                      return expand_kings(1ull << index);
                    });
  std::array<uint64_t, 64> data;
  std::copy_n(view.begin(), 64, data.begin());
  return data;
}();

const std::array<uint64_t, 64> lookup_knights = []() noexcept {
  std::array<uint64_t, 64> v;
  for (int i = 0; i < 64; ++i)
    v[i] = expand_knights(1ull << i);
  return v;
}();

const std::array<uint64_t, 64> lookup_pawns_w = []() noexcept {
  std::array<uint64_t, 64> v;
  for (int i = 0; i < 64; ++i)
    v[i] = expand_pawns(1ull << i, white_side{});
  return v;
}();

const std::array<uint64_t, 64> lookup_pawns_b = []() noexcept {
  std::array<uint64_t, 64> v;
  for (int i = 0; i < 64; ++i)
    v[i] = expand_pawns(1ull << i, black_side{});
  return v;
}();

const sliders_lookup lookup_rook_queen = []() noexcept {
  std::array<sliders_lookup::block, 64> blocks;

  for (int i = 0; i < 64; ++i) {
    uint64_t board = 1ull << i;
    uint64_t rooks = expand_rooks(board, ~0ull);
    //    uint64_t rooks = (mask_[0] & ~("a"_f | "h"_f)) | (mask_[1] & ~("1"_r | "8"_r));
    auto size = 1ull << std::popcount(rooks);
    blocks[i].data.resize(size);
    for (auto j = 0; j < size; ++j) {
      uint64_t blockers = blockers_permutation(j, rooks);
      uint64_t rooks2 = expand_rooks(board, ~blockers);
      blocks[i].data[j] = rooks2;
    }
    blocks[i].mask = rooks;
  }

  return sliders_lookup{blocks};
}();

const sliders_lookup lookup_bishop_queen = []() noexcept {
  std::array<sliders_lookup::block, 64> blocks;

  for (int i = 0; i < 64; ++i) {
    uint64_t board = 1ull << i;
    uint64_t bishops = expand_bishops(board, ~0ull);
    //    board_t bishops = (mask[2] | mask[3]) & ~("a"_f | "h"_f | "1"_r | "8"_r);
    auto size = 1ull << std::popcount(bishops);
    blocks[i].data.resize(size);
    for (auto j = 0; j < size; ++j) {
      uint64_t blockers = blockers_permutation(j, bishops);
      uint64_t bishops2 = expand_bishops(board, ~blockers);
      blocks[i].data[j] = bishops2;
    }
    blocks[i].mask = bishops;
  }

  return sliders_lookup{blocks};
}();
