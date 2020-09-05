#include "mps.h"

#include <iostream>

using namespace mps;

constexpr auto SocketAddressFamily = AddressFamily::IPv4;
constexpr auto SocketPort = 56789;

int main() {
  try {
    // build and bind a new TCP socket and print out the socket info.
    TCPServerSocket socket(SocketPort, SocketAddressFamily);
    std::cout << "Bound a new TCP socket with following details:" << std::endl;
    std::cout << "    local-ip: " << socket.getLocalIP() << std::endl;
    std::cout << "  local-port: " << socket.getLocalPort() << std::endl;
    std::cout << "    blocking: " << socket.isBlocking() << std::endl;
    std::cout << "     routing: " << socket.isRouting() << std::endl;
    std::cout << "    bufferin: " << socket.isBuffering() << std::endl;
    std::cout << " recvBufSize: " << socket.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << socket.getSendBufferSize() << std::endl;

    // wait for an incoming TCP client connection and then print client info.
    std::cout << "Waiting for incoming TCP client connection..." << std::endl;
    auto client = socket.accept();
    std::cout << "Received a TCP client with following details:" << std::endl;
    std::cout << "    local-ip: " << client.getLocalIP() << std::endl;
    std::cout << "  local-port: " << client.getLocalPort() << std::endl;
    std::cout << "   remote-ip: " << client.getRemoteIP() << std::endl;
    std::cout << " remote-port: " << client.getRemotePort() << std::endl;
    std::cout << "    blocking: " << client.isBlocking() << std::endl;
    std::cout << "     routing: " << client.isRouting() << std::endl;
    std::cout << "   buffering: " << client.isBuffering() << std::endl;
    std::cout << " recvBufSize: " << client.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << client.getSendBufferSize() << std::endl;

    // wait for incoming data by blocking and then print the message payload.
    std::cout << "Waiting for incoming TCP message..." << std::endl;
    auto data = client.receive();
    std::cout << "Received a TCP message with following content:" << std::endl;
    std::cout << "data: " << &data[0] << std::endl;

    // echo the received data back to caller to inform that we did receive it.
    std::cout << "Echoing the data back to client and closing..." << std::endl;
    client.send(data);
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
