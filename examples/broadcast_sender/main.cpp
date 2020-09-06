#include "mps.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace mps;

constexpr auto MessageCount = 100;
constexpr auto MessageInterval = std::chrono::seconds(1);

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
    UDPSocket socket(broadcastAddr.getFamily());
    socket.setBroadcasting(true);

    // broadcast messages N-times with the provided interval.
    std::cout << "Starting UDP broadcast to ";
    std::cout << broadcastAddr.getIP();
    std::cout << ":";
    std::cout << broadcastAddr.getPort();
    std::cout << std::endl;
    for (auto i = MessageCount; i > 0; i--) {
      socket.send(UDPPacket{ broadcastAddr, {'p','i','n','g','\0'} });
      std::cout << "[ping]" << std::endl;
      std::this_thread::sleep_for(MessageInterval);
    }
  } catch (const AddressException& e){
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
