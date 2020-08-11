#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "address_family.h"
#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    TCPSocket(AddressFamily addressFamily);
    TCPSocket(const TCPSocket& other) = delete;
    TCPSocket(TCPSocket&& other) noexcept = default;

    TCPSocket& operator=(const TCPSocket& other) = delete;
    TCPSocket& operator=(TCPSocket&& other) noexcept;

    virtual ~TCPSocket();
  };
}

#endif
