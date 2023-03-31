#pragma once

#include <variant>

struct move {
  struct king {};
  struct king_castle_short {};
  struct king_castle_long {};
  struct queen {};
  struct rook {};
  struct bishop {};
  struct knight {};
  struct pawn_single {};
  struct pawn_single_promote_queen {};
  struct pawn_single_promote_rook {};
  struct pawn_single_promote_bishop {};
  struct pawn_single_promote_knight {};
  struct pawn_double {};
  struct pawn_capture {};
  struct pawn_capture_promote_queen {};
  struct pawn_capture_promote_rook {};
  struct pawn_capture_promote_bishop {};
  struct pawn_capture_promote_knight {};
  struct pawn_capture_en_passant {};

  using tag_t = std::variant<
      king, king_castle_short, king_castle_long,
      queen,
      rook,
      bishop,
      knight,
      pawn_single, pawn_double, pawn_capture,
      pawn_single_promote_queen, pawn_single_promote_rook, pawn_single_promote_bishop, pawn_single_promote_knight,
      pawn_capture_promote_queen, pawn_capture_promote_rook, pawn_capture_promote_bishop, pawn_capture_promote_knight,
      pawn_capture_en_passant
  >;

  tag_t tag;
  char from;
  char to;
};
