#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <immintrin.h> // _pext

struct sliders_lookup {
  struct block {
    uint64_t mask;
    std::vector<uint64_t> data;
  };

  std::array<block, 64> blocks;

  constexpr uint64_t operator()(auto square, uint64_t occupied) const noexcept {
    const auto& block = blocks[square];
    const auto index = _pext_u64(occupied, block.mask);
    return block.data[index];
  }
};

extern const std::array<uint64_t, 64> lookup_kings;
extern const std::array<uint64_t, 64> lookup_knights;
extern const std::array<uint64_t, 64> lookup_pawns_w;
extern const std::array<uint64_t, 64> lookup_pawns_b;
extern const sliders_lookup lookup_rook_queen;
extern const sliders_lookup lookup_bishop_queen;
