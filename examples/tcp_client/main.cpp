#include "mps/tcp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto Port = 5555;
constexpr auto IPAddress = "127.0.0.1";

int main() {
  char buffer[BufferSize];
  std::string message = "hello";
  auto serverAddress = Address(IPAddress, Port);
  try {
    TCPSocket socket(AddressFamily::IPv4);
    socket.connect(serverAddress);
    std::cout << "Sending '" << message << "' to server..." << std::endl;
    socket.send(message);
    std::cout << "Waiting for server to respond..." << std::endl;
    socket.recv(buffer, BufferSize);
    buffer[4] = '\0';
    std::cout << "Received message '" << buffer << "' from the server" << std::endl;
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
