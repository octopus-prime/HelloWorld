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

size_t test::run() const {
  size_t count = 0;
  for (int i = 0; i < 3; ++i) {
    size_t n = position.perft(i + 1);
    if (n != expected[i])
      throw std::runtime_error(std::format("kaputtnik: d={}, n={}, e={}", i + 1, n, expected[i]));
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
    workers.emplace_back([test, &count, &slots]() {
      count += test.run();
      slots.release();
    });
  }
  return count;
}
