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

    std::cout << "KeepAlive enabled before: " << socket.isKeepAliveEnabled() << std::endl;
    socket.setKeepAliveEnabled(true);
    std::cout << "KeepAlive enabled after: " << socket.isKeepAliveEnabled() << std::endl;

    std::cout << "Socket receive buffer size before: " << socket.getReceiveBufferSize() << std::endl;
    socket.setReceiveBufferSize(1024);
    std::cout << "Socket receive buffer size after: " << socket.getReceiveBufferSize() << std::endl;

    std::cout << "Socket send buffer size before: " << socket.getSendBufferSize() << std::endl;
    socket.setSendBufferSize(1024);
    std::cout << "Socket send buffer size after: " << socket.getSendBufferSize() << std::endl;

    std::cout << "DontRoute enabled before : " << socket.isNonRouting() << std::endl;
    socket.setNonRouting(true);
    std::cout << "DontRoute enabled after: " << socket.isNonRouting() << std::endl;

    auto addr = socket.getLocalAddress();
    std::cout << "Bound addr='" << addr.getIPAddress() << "' port=" << addr.getPort() << std::endl;
    auto client = socket.accept();
    auto clientLocalAddr = client->getLocalAddress();
    auto clientRemoteAddr = client->getRemoteAddress();
    std::cout << "L-Client addr='" << clientLocalAddr.getIPAddress() << "' port=" << clientLocalAddr.getPort() << std::endl;
    std::cout << "R-Client addr='" << clientRemoteAddr.getIPAddress() << "' port=" << clientRemoteAddr.getPort() << std::endl;
    auto data = client->receive();
    std::vector<Byte> buffer(data);
    buffer.push_back('\0');
    std::cout << "echoing data: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    client->send(data);
  } catch (const SocketException& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
