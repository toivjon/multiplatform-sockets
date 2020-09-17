#include "mps.h"

#include <iostream>

using namespace mps;

int main(int argc, char* argv[]) {
  // sanity check to check that we have enough arguments.
  if (argc != 3) {
    std::cout << "Usage: <executable> <host> <port>" << std::endl;
    return -1;
  }

  try {
    // parse host and port from the commandline arguments.
    auto host = std::string(argv[1]);
    auto port = std::stoi(argv[2]);
    auto addr = Address(host, port);

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
    std::cout << " recvTimeout: " << socket.getReceiveTimeout() << std::endl;

    // send a simple datagram to the specified remote host IP address and port.
    std::cout << "Sending a simple UDP message to remote target:" << std::endl;
    std::cout << "  remote-ip: " << addr.getAddress() << std::endl;
    std::cout << "remote-port: " << addr.getPort() << std::endl;
    std::cout << "       data: " << "hello" << std::endl;
    socket.send(UDPPacket{ addr, { 'h','e','l','l','o', '\0' } });

    // wait for the incoming data by blocking and the print datagram info.
    std::cout << "Waiting for the remote target to echo back..." << std::endl;
    UDPPacket packet;
    socket.receive(packet);
    std::cout << "Received a UDP datagram with following details:" << std::endl;
    std::cout << "  remote-ip: " << packet.getIP() << std::endl;
    std::cout << "remote-port: " << packet.getPort() << std::endl;
    std::cout << "       data: " << &packet.getData()[0] << std::endl;
  } catch (const AddressException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
