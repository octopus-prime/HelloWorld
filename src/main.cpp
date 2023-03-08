#include <string_view>
#include <functional>
#include <numeric>
#include <ranges>
#include <iostream>

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

//  auto r = 0ull;
//  for (size_t i = 0; i < length; i += 2)
//    r |= operator"" _r(data + i + 1, 1) & operator"" _f(data + i, 1);
//  return r;
}

static_assert("a1b3"_b == 131073, "check op_b");

int main() {
  std::cout << "Hello world" << std::endl;
}
