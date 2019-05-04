#include <utvpi_oa_fm.h>
#include <iostream>

int main() {
  fm::System<int> system;
  system.read(std::cin);
  system.print(std::cout);
  system.removeRedundantConstraints();
  system.print(std::cout);
  std::cout << "Over Approximation" << std::endl;
  system.printOA(std::cout, false);
  return 0;
}
