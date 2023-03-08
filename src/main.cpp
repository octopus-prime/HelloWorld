#include "literal.hpp"
#include <iostream>

int main() {
  std::cout << "Hello world" << std::endl;
  std::cout << "a"_f << std::endl;
  std::cout << "1"_r << std::endl;
  std::cout << "a1b3"_b << std::endl;
  std::cout << "a1"_s << std::endl;
  std::cout << "b3"_s << std::endl;
  std::cout << "h8"_s << std::endl;
}
