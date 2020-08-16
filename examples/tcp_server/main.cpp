#include "mps/tcp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto Port = 5555;
constexpr auto IPAddress = "127.0.0.1";

int main() {
  /*
  char buffer[BufferSize];
  std::string message = "thx!";
  try {
    TCPSocket socket(AddressFamily::IPv4);
    socket.bind(Address(IPAddress, Port));
    std::cout << "Listening for incoming messages on port " << Port << std::endl;
    socket.listen(1);
    auto conn = socket.accept();
    auto senderAddress = socket.recvFrom(buffer, BufferSize);
    buffer[5] = '\0';
    std::cout << "Received message '" << buffer << "' from the client" << std::endl;
    std::cout << "Sending a '" << message << "' message to client" << std::endl;
    socket.sendTo(senderAddress, message);
  } catch (SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  */
  return 0;
}
