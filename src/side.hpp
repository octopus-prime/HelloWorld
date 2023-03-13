#pragma once

struct white_side {};
struct black_side {};

template <typename side> struct flipper;

template <> struct flipper<white_side> {
  using side = black_side;
};

template <> struct flipper<black_side> {
  using side = white_side;
};

template <typename side> using flip = flipper<side>::side;
