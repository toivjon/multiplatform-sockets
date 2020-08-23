#include "mps/udp_socket.h"
#include "mps/exception.h"

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
    UDPSocket socket;
    auto localAddress = socket.getAddress();
    std::cout << "Bound addr='" << localAddress.getIPAddress() << "' port=" << localAddress.getPort() << std::endl;
    socket.send(UDPPacket{ addr, { 'h','e','l','l','o' } });
    auto packet = socket.recv(BufferSize);
    std::cout << "response packet: " << packet.toString() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
