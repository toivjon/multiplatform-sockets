#include "mps/udp_socket.h"

#include <iostream>

using namespace mps;

int main() {
    char buffer[1024];

    UDPSocket socket(AddressFamily::IPv4);
    
    std::cout << "Sending 'hello' message to server..." << std::endl;
    socket.sendTo(Address("127.0.0.1", 5555), reinterpret_cast<const void*>("hello"), 5);
    
    socket.recvFrom(buffer, 1024);
    buffer[5] = '\0';
    std::cout << "Message from the server: " << buffer << std::endl; 
    return 0;
}