#include "mps.h"

#include <iostream>

using namespace mps;

int main() {
  try {
    Address address1("100.0.0.0", 12345);
    Address address2(std::move(address1));
    std::cout << address1 << std::endl;
    std::cout << address2 << std::endl;
  } catch (const SocketException& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
