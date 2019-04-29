#include <utvpi_oa_fm.h>
#include <iostream>

int main() {
  fm::System<int> system;
  system.read(std::cin);
  system.print(std::cout);
  auto system0 = system.removeVar(0);
  auto system1 = system.removeVar(1);
  system0.print(std::cout);
  system1.print(std::cout);
  system0.removeVar(1).print(std::cout);
  system1.removeVar(1).print(std::cout);
  return 0;
}