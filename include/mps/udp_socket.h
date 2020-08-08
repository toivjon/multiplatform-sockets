#ifndef MPS_UDP_SOCKET_H
#define MPS_UDP_SOCKET_H

#include "socket.h"

namespace mps {
  class UDPSocket : public Socket {
  public:
    UDPSocket();
    UDPSocket(const UDPSocket& other) = delete;
    UDPSocket(UDPSocket&& other) noexcept;

    UDPSocket& operator=(const UDPSocket& other) = delete;
    UDPSocket& operator=(UDPSocket&& other) noexcept;

    virtual ~UDPSocket();
  };
}

#endif
