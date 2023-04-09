#include "position.hpp"
#include "move.hpp"
#include "literal.hpp"
#include "side.hpp"
#include "io.hpp"
#include <cstdint>
#include <ranges>
#include <string_view>
#include <regex>
#include <atomic>
#include <thread>
#include <tuple>

position::position() :
  node{
    "12"_r,
    "78"_r,
    "e1e8"_b,
    "a1d1h1a8d8h8"_b,
    "c1d1f1c8d8f8"_b,
    "b1g1b8g8"_b,
    "27"_r,
    "a1h1a8h8"_b,
    {}},
  color{white_side{}}
{}

static const std::regex re2("(.*)/(.*)/(.*)/(.*)/(.*)/(.*)/(.*)/(.*) ([wb]) ([-KQkq]+) ([-a-h1-8]+)( \\d+)?( \\d+)?");

position::position(std::string_view fen) : node{}, color{} {
  std::cmatch m;
  if (!std::regex_search(fen.data(), m, re2))
    throw std::runtime_error("fen not matched by regex");
  for (int i = 0; i < 8; ++i) {
    int j = 0;
    for (auto ch : m[8 - i].str()) {
      if (std::isdigit(ch))
        j += ch - '0';
      else {
        uint64_t b = 1ull << (8 * i + j);
        switch (std::tolower(ch)) {
        case 'r':
          node.rook_queen |= b;
          break;
        case 'b':
          node.bishop_queen |= b;
          break;
        case 'q':
          node.rook_queen |= b;
          node.bishop_queen |= b;
          break;
        case 'k':
          node.king |= b;
          break;
        case 'p':
          node.pawn |= b;
          break;
        case 'n':
          node.knight |= b;
          break;
        }
        if (std::isupper(ch))
          node.white |= b;
        else
          node.black |= b;
        ++j;
      }
    }
  }
  color = m[9].str()[0] == 'w' ? color_t{white_side{}} : color_t{black_side{}};
  if (m[10].compare("-")) {
    for (auto ch : m[10].str()) {
      switch (ch) {
      case 'K':
        node.castle |= "h1"_b;
        break;
      case 'Q':
        node.castle |= "a1"_b;
        break;
      case 'k':
        node.castle |= "h8"_b;
        break;
      case 'q':
        node.castle |= "a8"_b;
        break;
      }
    }
  }
  if (m[11].compare("-")) {
    auto file = m[11].str()[0] - 'a';
    auto rank = m[11].str()[1] - '1';
    auto square = 8 * rank + file;
    node.en_passant = 1ull << square;
  }
}

std::string_view intend = "                                         ";

template <typename side>
size_t search(const node& node, int depth) noexcept {
  //  if (depth == 0)
  //    return 1;
  size_t count = 0;
  move moves[256];
  for (const move& move : node.generate<side>(moves)) {
    const struct node succ(node, move, side{});
    if (!succ.check<side>())
      count += depth > 1 ? search<flip<side>>(succ, depth - 1) : 1;
  }
  return count;
}

template <typename side>
std::tuple<size_t, size_t> search2(const node& node, int depth) noexcept {
  //  if (depth == 0)
  //    return 1;
  size_t count = 0;
  size_t checks = 0;
  move moves[256];
  for (const move& move : node.generate<side>(moves)) {
    const struct node succ(node, move, side{});
    if (!succ.check<side>()) {
      if (depth > 1) {
        auto [cn,cs] = search2<flip<side>>(succ, depth - 1);
        count += cn;
        checks += cs;
      } else
        ++count;
    } else
      ++checks;
  }
  return {count, checks};
}

template <typename side>
void print(const node& node, int depth, int height) {
  if (depth == 0)
    return;
  move moves[256];
  for (const move& move : node.generate<side>(moves)) {
    const struct node succ(node, move, side{});
//    if (!succ.check<side>()) {
      std::cout << std::string_view{intend.data(), intend.data() + 2 * height} << move << std::endl;
      print<flip<side>>(succ, depth - 1, height + 1);
//    }
  }
}

size_t position::perft(int depth) const noexcept {
  return std::visit([this, depth] <typename side>(side) noexcept {
    return search<side>(node, depth);
  }, color);
}

std::tuple<size_t, size_t> position::perft2(int depth) const noexcept {
  return std::visit([this, depth] <typename side>(side) noexcept {
    return search2<side>(node, depth);
  }, color);
}

size_t position::perftp(int depth) const noexcept {
  std::atomic_uint64_t count = 0;
  std::vector<std::jthread> todo;
  for (int i = 0; i < std::jthread::hardware_concurrency(); ++i) {
    todo.emplace_back([this,depth,&count]() noexcept {
      count += perft(depth);
    });
  }
  for (auto &&y : todo)
    y.join();
  return count;
}

void position::printt(int depth) const noexcept {
  std::visit([this, depth] <typename side>(side){
    print<side>(node, depth, 0);
  }, color);
}
