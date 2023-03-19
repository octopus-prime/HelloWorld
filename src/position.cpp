#include "position.hpp"
#include "side.hpp"
#include "move.hpp"
#include <string_view>

position::position() : node(), color(white_side{}) {}

position::position(std::string_view) : node(), color(white_side{}) {
}

template <typename side>
size_t search(const node& node, int depth) {
  if (depth == 0)
    return 1;
  size_t count = 0;
  move moves[256];
  for (const move& move : node.generate<side>(moves)) {
    const struct node succ(node, move, side{});
//    if (!succ.check<side>())
      count += search<flip<side>>(succ, depth - 1);
  }
  return count;
}

size_t position::perft(int depth) const noexcept {
  return std::visit([this, depth] <typename side>(side){
    return search<side>(node, depth);
  }, color);
}
