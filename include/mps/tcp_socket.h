#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    TCPSocket();
    TCPSocket(const TCPSocket& other) = delete;
    TCPSocket(TCPSocket&& other) noexcept;

    TCPSocket& operator=(const TCPSocket& other) = delete;
    TCPSocket& operator=(TCPSocket&& other) noexcept;

    virtual ~TCPSocket();
  };
}

#endif
