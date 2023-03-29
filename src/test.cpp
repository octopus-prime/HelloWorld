#include "test.hpp"
#include <array>
#include <atomic>
#include <cstddef>
#include <regex>
#include <string_view>
#include <thread>
#include <semaphore>
#include <fstream>
#include <iostream>
#include <format>

static const std::regex re2("(.*),(.*),(.*),(.*),(.*),(.*),(.*)");

test::test(std::string_view row) {
  std::cmatch m;
  if (!std::regex_search(row.data(), m, re2))
    throw std::runtime_error("broken row");
  position = {m[1].str()};
  for (int i = 0; i < 6; ++i) {
    expected[i] = std::atoll(m[2 + i].str().data());
  }
}

std::expected<size_t, std::string> test::run() const noexcept {
  size_t count = 0;
  for (int i = 0; i < 5; ++i) {
    size_t n = position.perft(i + 1);
    if (n != expected[i])
      return std::unexpected(std::format("kaputtnik: d={}, n={}, e={}", i + 1, n, expected[i]));
    count += n;
  }
  return count;
}

size_t test::run(std::string_view file) {
  std::atomic_size_t count{};
  std::vector<std::jthread> workers{};
  std::counting_semaphore slots{std::jthread::hardware_concurrency()};
  std::ifstream in{file.data()};
  while (!in.eof()) {
    slots.acquire();
    char line[256];
    in.getline(line, 256);
    std::cout << line << std::endl;
    const struct test test{line};
    workers.emplace_back([test, &count, &slots]() noexcept {
      const auto result = test.run();
      if (result.has_value()) {
        count += result.value();
      } else {
        std::cerr << result.error() << std::endl;
      }
      slots.release();
    });
  }
  return count;
}
