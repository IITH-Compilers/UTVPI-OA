#include <utvpi_oa_fm.h>
#include <iostream>

int main() {
  fm::System<int> system;
  system.read(std::cin);
  system.print(std::cout);
  system.printOA(std::cout, true);
  return 0;
}
