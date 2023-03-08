#pragma once

#include <string_view>
#include <functional>
#include <numeric>
#include <ranges>

constexpr auto operator""_f(const char* data, size_t length) noexcept {
  constexpr auto mask = 0b100000001000000010000000100000001000000010000000100000001ull;
  const auto view = std::string_view{data, length};
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [](char ch) {
    return mask << (ch - 'a');
  });
}

constexpr auto operator""_r(const char* data, size_t length) noexcept {
  constexpr auto mask = 0b11111111ull;
  const auto view = std::string_view{data, length};
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [](char ch) {
    return mask << (ch - '1') * 8;
  });
}

constexpr auto operator""_b(const char* data, size_t length) noexcept {
  const auto view = std::string_view{data, length} | std::views::chunk(2);
  return std::transform_reduce(view.begin(), view.end(), 0ull, std::bit_or{}, [](auto&& ch) {
    return operator""_f(&ch[0], 1) & operator""_r(&ch[1], 1);
  });
}

constexpr auto operator""_s(const char* data, size_t length) noexcept {
  const auto view = std::string_view{data, length} | std::views::take(2);
  return std::countr_zero(operator""_b(view.data(), view.size()));
}
