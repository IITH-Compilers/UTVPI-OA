#include <utvpi_oa_fm.h>
#include <iostream>

int main() {
  fm::System<int> system;
  system.read(std::cin);
  system.print(std::cout);
  system.removeRedundantConstraints();
  system.print(std::cout);
  std::cout << "Over Approximation using LP" << std::endl;
  system.printLPOA(std::cout);
  std::cout << "Over Approximation using FM" << std::endl;
  system.printFMOA(std::cout);
  return 0;
}
