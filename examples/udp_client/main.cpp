#include "mps/udp_socket.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto Port = 5555;
constexpr auto IPAddress = "127.0.0.1";

int main() {
    char buffer[BufferSize];

    UDPSocket socket(AddressFamily::IPv4);
    
    std::cout << "Sending 'hello' message to server..." << std::endl;
    socket.sendTo(Address(IPAddress, Port), reinterpret_cast<const void*>("hello"), 5);
    
    socket.recvFrom(buffer, BufferSize);
    buffer[5] = '\0';
    std::cout << "Message from the server: " << buffer << std::endl; 
    return 0;
}
