#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketAddressFamily = AddressFamily::IPv6;
constexpr auto SocketPort = 56789;

int main() {
  try {
    // build a new UDP socket and print out the bound address.
    UDPSocket socket(SocketPort, SocketAddressFamily);
    std::cout << "Server: " << socket.getLocalAddress() << std::endl;

    auto packet = socket.receive(BufferSize);
    std::vector<uint8_t> buffer(packet.data);
    buffer.push_back('\0');
    std::cout << "response: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    socket.send(packet);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
