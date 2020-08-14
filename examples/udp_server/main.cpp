#include "mps/udp_socket.h"

#include <iostream>

using namespace mps;

int main() {
    char buffer[1024];

    UDPSocket socket(AddressFamily::IPv4);
    socket.bind(Address("127.0.0.1", 5555));

    std::cout << "Listening for incoming messages on port " << 5555 << std::endl;
    auto senderAddress = socket.recvFrom(buffer, 1024);
    buffer[5] = '\0';
    std::cout << "Message from the client: " << buffer << std::endl;

    std::cout << "Sending a 'thx!' message to client" << std::endl;
    socket.sendTo(senderAddress, reinterpret_cast<const void*>("thx!"), 4);
    socket.recvFrom(buffer, 1024);
    
    return 0;
}