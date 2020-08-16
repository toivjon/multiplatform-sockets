#include "mps/udp_socket.h"
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
    UDPSocket socket(AddressFamily::IPv4);
    std::cout << "Sending '" << message << "' to server..." << std::endl;
    socket.sendTo(serverAddress, message);
    std::cout << "Waiting for server to respond..." << std::endl;
    socket.recvFrom(buffer, BufferSize);
    buffer[4] = '\0';
    std::cout << "Received message '" << buffer << "' from the server" << std::endl;
  } catch (SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
