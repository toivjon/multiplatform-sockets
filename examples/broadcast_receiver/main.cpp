#include "mps.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace mps;

constexpr auto MessageCount = 5;
constexpr auto MessageInterval = std::chrono::seconds(1);

// TODO just take address family as a cmdline parameter instead of an IP

int main(int argc, char* argv[]) {
  // sanity check to check that we have enough arguments.
  if (argc != 3) {
    std::cout << "Usage: <executable> <broadcast-ip> <broadcast-port>" << std::endl;
    return -1;
  }

  try {
    // parse host and port from the commandline arguments.
    auto host = std::string(argv[1]);
    auto port = std::stoi(argv[2]);
    auto broadcastAddr = Address(host, port);

    // build and bind a new UDP socket and enable broadcasting.
    UDPSocket socket(broadcastAddr.getFamily(), broadcastAddr.getPort());
    socket.setBroadcasting(true);

    // wait for an broadcast message to arrive and print its contents.
    std::cout << "Waiting for UDP message from port ";
    std::cout << socket.getLocalPort();
    std::cout << std::endl;
    UDPPacket packet;
    socket.receive(packet);
    std::cout << "Received a UDP datagram with the following details:" << std::endl;
    std::cout << "  remote-ip: " << packet.getIP() << std::endl;
    std::cout << "remote-port: " << packet.getPort() << std::endl;
    std::cout << "       data: " << "hello" << std::endl;
  } catch (const AddressException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
