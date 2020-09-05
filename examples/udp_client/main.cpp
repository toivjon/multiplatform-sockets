#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;

int main(int argc, char* argv[]) {
  // sanity check to check that we have enough arguments.
  if (argc != 3) {
    std::cout << "Usage: <executable> <host> <port>" << std::endl;
    return -1;
  }

  // parse host and port from the commandline arguments.
  auto host = std::string(argv[1]);
  auto port = std::stoi(argv[2]);
  auto addr = Address(host, port);

  try {
    // build a new UDP socket and print out the bound address.
    UDPSocket socket(addr.getFamily());
    std::cout << "Client: " << socket.getLocalAddress() << std::endl;

    socket.send(UDPPacket{ addr, { 'h','e','l','l','o' } });
    auto packet = socket.receive(BufferSize);
    std::vector<uint8_t> buffer(packet.data);
    buffer.push_back('\0');
    std::cout << "response: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
