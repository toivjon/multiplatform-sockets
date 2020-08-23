#include "mps/tcp_server_socket.h"
#include "mps/exception.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    TCPServerSocket socket(SocketPort);
    std::cout << "Nagle enabled before: " << socket.isNagleEnabled() << std::endl;
    socket.setNagleEnabled(false);
    std::cout << "Nagle enabled after: " << socket.isNagleEnabled() << std::endl;

    auto addr = socket.getLocalAddress();
    std::cout << "Bound addr='" << addr.getIPAddress() << "' port=" << addr.getPort() << std::endl;
    auto client = socket.accept();
    auto clientLocalAddr = client->getLocalAddress();
    auto clientRemoteAddr = client->getRemoteAddress();
    std::cout << "L-Client addr='" << clientLocalAddr.getIPAddress() << "' port=" << clientLocalAddr.getPort() << std::endl;
    std::cout << "R-Client addr='" << clientRemoteAddr.getIPAddress() << "' port=" << clientRemoteAddr.getPort() << std::endl;
    auto data = client->recv();
    std::vector<Byte> buffer(data);
    buffer.push_back('\0');
    std::cout << "echoing data: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    client->send(data);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
