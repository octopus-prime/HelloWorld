#include "literal.hpp"

static_assert("a"_f == 72340172838076673ull, "file a");
static_assert("1"_r == 255ull, "rank 1");
static_assert("a1b3"_b == 131073ull, "board a1b3");
static_assert("a1"_s == 0, "square a1");
static_assert("b3"_s == 17, "square b3");
static_assert("h8"_s == 63, "square h8");
