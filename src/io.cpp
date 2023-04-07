#include "io.hpp"
#include <string_view>

struct square_ {
  char view[3];

  square_(int s) {
    view[0] = 'a' + s % 8;
    view[1] = '1' + s / 8;
    view[2] = 0;
  }
};

void print(std::ostream& stream, int from, int to, move::king) {
  stream << 'K' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::king_castle_short) {
  stream << "O-O";
}

void print(std::ostream& stream, int from, int to, move::king_castle_long) {
  stream << "O-O-O";
}

void print(std::ostream& stream, int from, int to, move::queen) {
  stream << 'Q' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::rook) {
  stream << 'R' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::bishop) {
  stream << 'B' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::knight) {
  stream << 'N' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::pawn_single) {
  stream << 'P' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::pawn_double) {
  stream << 'P' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::pawn_capture) {
  stream << 'P' << square_(from).view << square_(to).view;
}

void print(std::ostream& stream, int from, int to, move::pawn_single_promote_queen) {
  stream << 'P' << square_(from).view << square_(to).view << 'Q';
}

void print(std::ostream& stream, int from, int to, move::pawn_capture_promote_queen) {
  stream << 'P' << square_(from).view << square_(to).view << 'Q';
}

void print(std::ostream& stream, int from, int to, move::pawn_single_promote_rook) {
  stream << 'P' << square_(from).view << square_(to).view << 'R';
}

void print(std::ostream& stream, int from, int to, move::pawn_capture_promote_rook) {
  stream << 'P' << square_(from).view << square_(to).view << 'R';
}

void print(std::ostream& stream, int from, int to, move::pawn_single_promote_bishop) {
  stream << 'P' << square_(from).view << square_(to).view << 'B';
}

void print(std::ostream& stream, int from, int to, move::pawn_capture_promote_bishop) {
  stream << 'P' << square_(from).view << square_(to).view << 'B';
}

void print(std::ostream& stream, int from, int to, move::pawn_single_promote_knight) {
  stream << 'P' << square_(from).view << square_(to).view << 'N';
}

void print(std::ostream& stream, int from, int to, move::pawn_capture_promote_knight) {
  stream << 'P' << square_(from).view << square_(to).view << 'N';
}

void print(std::ostream& stream, int from, int to, move::pawn_capture_en_passant) {
  stream << 'P' << square_(from).view << square_(to).view << "ep";
}

std::ostream& operator<<(std::ostream& stream, const move& move) {
  std::visit([&](auto&& tag){
    print(stream, move.from, move.to, tag);
  }, move.tag);
  return stream;
}
