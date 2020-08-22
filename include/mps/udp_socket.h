#ifndef MPS_UDP_SOCKET_H
#define MPS_UDP_SOCKET_H

#include "address_family.h"
#include "socket.h"

/*
namespace mps {
  class UDPSocket : public Socket {
  public:
    UDPSocket(AddressFamily addressFamily);
    UDPSocket(const UDPSocket& other) = delete;
    UDPSocket(UDPSocket&& other) noexcept = default;

    UDPSocket& operator=(const UDPSocket& other) = delete;
    UDPSocket& operator=(UDPSocket&& other) noexcept;

    virtual ~UDPSocket();

    void sendTo(const Address& address, const void* data, size_t dataSize);
    void sendTo(const Address& address, const std::string& data);
    Address recvFrom(void* data, size_t maxDataSize);
  };
}
*/

#endif
