#include "mps/tcp_client_socket.h"
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
    TCPClientSocket socket(addr);
    auto lAddr = socket.getLocalAddress();
    auto rAddr = socket.getRemoteAddress();
    std::cout << "Local addr='" << lAddr.getIPAddress() << "' port=" << lAddr.getPort() << std::endl;
    std::cout << "Remote addr='" << rAddr.getIPAddress() << "' port=" << rAddr.getPort() << std::endl;
    socket.send({ 'h','e','l','l','o' });
    auto data = socket.recv(BufferSize);
    std::vector<Byte> buffer(data);
    buffer.push_back('\0');
    std::cout << "response: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
