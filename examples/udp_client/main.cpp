#include "mps.h"

#include <iostream>

using namespace mps;

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

  try {
    // build and bind a new UDP socket and print out the socket info.
    UDPSocket socket(addr.getFamily());
    std::cout << "Bound a new UDP socket with following details:" << std::endl;
    std::cout << "    local-ip: " << socket.getLocalIP() << std::endl;
    std::cout << "  local-port: " << socket.getLocalPort() << std::endl;
    std::cout << "    blocking: " << socket.isBlocking() << std::endl;
    std::cout << "     routing: " << socket.isRouting() << std::endl;
    std::cout << "broadcasting: " << socket.isBroadcasting() << std::endl;
    std::cout << " recvBufSize: " << socket.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << socket.getSendBufferSize() << std::endl;

    // send a simple datagram to the specified remote host IP address and port.
    std::cout << "Sending a simple UDP message to remote target:" << std::endl;
    std::cout << "  remote-ip: " << addr.getIP() << std::endl;
    std::cout << "remote-port: " << addr.getPort() << std::endl;
    std::cout << "       data: " << "hello" << std::endl; // TODO constant
    socket.send(UDPPacket{ addr, { 'h','e','l','l','o', '\0' } });

    // wait for the incoming data by blocking and the print datagram info.
    auto packet = socket.receive();
    std::cout << "response: " << reinterpret_cast<const char*>(&packet.data[0]) << std::endl;
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    // std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
