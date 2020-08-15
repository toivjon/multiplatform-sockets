#include "mps/udp_socket.h"

#include <iostream>

using namespace mps;

constexpr auto BufferSize = 1024;
constexpr auto Port = 5555;
constexpr auto IPAddress = "127.0.0.1";

int main() {
    char buffer[BufferSize];

    UDPSocket socket(AddressFamily::IPv4);
    socket.bind(Address(IPAddress, Port));

    std::cout << "Listening for incoming messages on port " << Port << std::endl;
    auto senderAddress = socket.recvFrom(buffer, BufferSize);
    buffer[5] = '\0';
    std::cout << "Message from the client: " << buffer << std::endl;

    std::cout << "Sending a 'thx!' message to client" << std::endl;
    socket.sendTo(senderAddress, reinterpret_cast<const void*>("thx!"), 4);
    socket.recvFrom(buffer, BufferSize);
    
    return 0;
}
