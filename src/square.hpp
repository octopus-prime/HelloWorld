#pragma once

#include <cstdint>
#include <string_view>
#include <ostream>

class square {
  using storage_t = std::uint8_t;

  storage_t index;

public:
  constexpr square(std::integral auto index) noexcept
      : index{std::forward<storage_t>(index)} {}

  constexpr square(std::integral auto file, std::integral auto rank) noexcept
      : square{8 * rank + file} {}

  constexpr square(std::string_view string)
      : square{parse(string)} {
  }
  
  constexpr operator storage_t() const noexcept {
    return index;
  }

  constexpr auto file() const noexcept {
    return index % 8;
  }

  constexpr auto rank() const noexcept {
    return index / 8;
  }
  
  static constexpr square parse(std::string_view string) {
    return {string.at(0) - 'a', string.at(1) - '1'};
  }
};

constexpr square operator""_s2(const char* data, size_t length) {
  return std::string_view{data, length};
}

std::ostream& operator<<(std::ostream& stream, square square);
