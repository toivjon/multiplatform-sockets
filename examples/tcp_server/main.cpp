#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto SocketPort = 56789;

int main() {
  try {
    // build a new TCP socket and print out the bound address.
    TCPServerSocket socket(SocketPort);
    std::cout << "Server: " << socket.getLocalAddress() << std::endl;

    auto client = socket.accept();
    std::cout << "Client local  IP=" << client.getLocalIP() << " port=" << client.getLocalPort() << std::endl;
    std::cout << "Client remote IP=" << client.getRemoteIP() << " port=" << client.getRemotePort() << std::endl;
    client.setKeepAlive(true);

    auto data = client.receive();
    std::vector<uint8_t> buffer(data);
    buffer.push_back('\0');
    std::cout << "echoing data: " << reinterpret_cast<const char*>(&buffer[0]) << std::endl;
    client.send(data);
  } catch (const AddressException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
