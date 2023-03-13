#pragma once

#include "move.hpp"
#include "node.hpp"
#include <span>

template<typename side>
std::span<move> generate(const node& node, std::span<move, 256> moves) noexcept;
