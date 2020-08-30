#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    TCPServerSocket socket(SocketPort);
    auto client = socket.accept();

    auto data = client.receive();
    Bytes buffer(data);
    buffer.push_back('\0');
    std::cout << "echoing data: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    client.send(data);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
