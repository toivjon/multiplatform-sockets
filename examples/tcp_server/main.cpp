#include "mps/tcp_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto Port = 5555;
constexpr auto IPAddress = "127.0.0.1";

int main() {
  char buffer[BufferSize];
  std::string message = "thx!";
  try {
    TCPSocket socket(AddressFamily::IPv4);
    socket.bind(Port);
    socket.listen(1);
    std::cout << "Listening for incoming messages on port " << Port << std::endl;
    auto conn = socket.accept();
    TCPSocket client(conn.handle, conn.address.getAddressFamily());
    auto byteCount = client.recv(buffer, BufferSize);
    buffer[byteCount] = '\0';
    std::cout << "Received message '" << buffer << "' from the client" << std::endl;
    std::cout << "Sending a '" << message << "' message to client" << std::endl;
    client.send(message);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
