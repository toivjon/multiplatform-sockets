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

    // build and connect the client into destination and print bound addresses.
    TCPClientSocket socket(addr);
    std::cout << "Connected a TCP socket with following details:" << std::endl;
    std::cout << "    local-ip: " << socket.getLocalIP() << std::endl;
    std::cout << "  local-port: " << socket.getLocalPort() << std::endl;
    std::cout << "   remote-ip: " << socket.getRemoteIP() << std::endl;
    std::cout << " remote-port: " << socket.getRemotePort() << std::endl;
    std::cout << "    blocking: " << socket.isBlocking() << std::endl;
    std::cout << "     routing: " << socket.isRouting() << std::endl;
    std::cout << "   buffering: " << socket.isBuffering() << std::endl;
    std::cout << " recvBufSize: " << socket.getReceiveBufferSize() << std::endl;
    std::cout << " sendBufSize: " << socket.getSendBufferSize() << std::endl;

    // send a single message to the specified remote host IP address and port.
    std::cout << "Sending a simple TCP message to remote server:" << std::endl;
    std::cout << "data: " << "hello" << std::endl;
    socket.send({ 'h','e','l','l','o', '\0' });

    // wait for the incoming data by blocking and print message contents.
    std::cout << "Waiting for the server to echo message back..." << std::endl;
    auto data = socket.receive();
    std::cout << "Received a TCP message with following content:" << std::endl;
    std::cout << "data: " << &data[0] << std::endl;
  } catch (const AddressException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  } catch (const SocketException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
  return 0;
}
