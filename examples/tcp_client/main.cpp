#include "mps/tcp_socket.h"
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

  char buffer[BufferSize];
  std::string message = "hello";
  try {
    TCPSocket socket(addr.getAddressFamily());
    socket.connect(addr);
    std::cout << "Sending '" << message << "' to server..." << std::endl;
    socket.send(message);
    std::cout << "Waiting for server to respond..." << std::endl;
    auto byteCount = socket.recv(buffer, BufferSize);
    buffer[byteCount] = '\0';
    std::cout << "Received message '" << buffer << "' from the server" << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
