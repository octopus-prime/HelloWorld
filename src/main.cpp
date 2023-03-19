#include "literal.hpp"
#include "generator.hpp"
#include "executer.hpp"
#include "position.hpp"
#include "io.hpp"
#include "side.hpp"
#include <atomic>
#include <chrono>
#include <thread>

template <typename side>
size_t perft(const node& node, int depth) {
  if (depth == 0)
    return 1;
  size_t count = 0;
  move moves[256];
  for (const move& move : node.generate<side>(moves)) {
//    std::cout << move << std::endl;
    const struct node succ(node, move, side{});
    if (!succ.check<side>())
      count += perft<flip<side>>(succ, depth - 1);
  }
  return count;
}

int main() {
  std::cout << sizeof(move) << std::endl;

  const position position;
//  const node node;//{"foo"};
//  move moves[256];
//  for (const move& move : node.generate<white_side>(moves)) {
//    std::cout << move << std::endl;
//    const struct node succ(node, move, white_side{});
//    for (const struct move& move : node.generate<black_side>(moves)) {
//      std::cout << "  " << move << std::endl;
//      const struct node succ(node, move, black_side{});
//    }
//  }

  std::atomic_uint64_t count = 0;
  auto time0 = std::chrono::high_resolution_clock::now();

  constexpr auto depth = 6;
  /*
  std::vector<std::jthread> todo;
  for (int i = 0; i < 24; ++i) {
    todo.emplace_back([&position,&count](){
      count += position.perft(depth);
    });
  }
  for (auto &&y : todo)
    y.join();
*/
  count = position.perft(depth);
  auto time1 = std::chrono::high_resolution_clock::now();
  const auto time =
      std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(
          time1 - time0);
  std::cout << "n = " << count << std::endl;
  std::cout << "t = " << time.count() << std::endl;
  std::cout << "p = " << size_t(count / time.count()) << std::endl;

  std::cout << "Hello world" << std::endl;
  std::cout << "a"_f << std::endl;
  std::cout << "1"_r << std::endl;
  std::cout << "a1b3"_b << std::endl;
  std::cout << "a1"_s << std::endl;
  std::cout << "b3"_s << std::endl;
  std::cout << "h8"_s << std::endl;
}
