#include "test.hpp"
#include <array>
#include <atomic>
#include <cstddef>
#include <regex>
#include <string_view>
#include <thread>
#include <fstream>
#include <iostream>

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
  for (int i = 0; i < 6; ++i) {
    size_t n = position.perft(i + 1);
    if (n != expected[i])
      throw std::runtime_error("kaputtnik: " + std::to_string(i + 1));
    count += n;
  }
  return count;
}

size_t test::run(std::string_view file) {
  std::atomic_size_t count = 0;
  std::vector<std::jthread> todo;
  std::counting_semaphore slots(1);//std::jthread::hardware_concurrency());
  std::ifstream in(file.data());
  while (!in.eof()) {
    char line[1024];
    in.getline(line, 1024);
    std::cout << line << std::endl;
    const test t{line};
    count += t.run();
//    todo.emplace_back([t, &count, &slots]() {
//      slots.acquire();
//      count += t.run();
//      slots.release();
//    });
  }
  in.close();
  for (auto &&y : todo)
    y.join();
  return count;
}
