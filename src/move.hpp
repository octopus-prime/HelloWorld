#pragma once

#include <cstdint>
#include <variant>

struct move {
  struct king {};
  struct castle_short {};
  struct castle_long {};
  struct queen {};
  struct rook {};
  struct bishop {};
  struct knight {};
  struct knight_capture {};
  struct pawn {};
  struct promote_queen {};
  struct promote_rook {};
  struct promote_bishop {};
  struct promote_knight {};
  struct en_passant {};

  using tag_t = std::variant<king, castle_short, castle_long, queen, rook, bishop, knight, /*knight_capture,*/ pawn, promote_queen, promote_rook, promote_bishop, promote_knight, en_passant>;

  tag_t tag;
  char from;
  char to;
};
