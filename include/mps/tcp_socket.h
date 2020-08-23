#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "address.h"
#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    TCPSocket(SocketHandle handle);
    TCPSocket(AddressFamily af);
    TCPSocket(const TCPSocket& rhs) = delete;
    TCPSocket(TCPSocket&& rhs) noexcept = default;

    TCPSocket& operator=(const TCPSocket& rhs) = delete;
    TCPSocket& operator=(TCPSocket&& rhs) noexcept = default;

    virtual ~TCPSocket() = default;

    void setNagleEnabled(bool enabled);
    bool isNagleEnabled() const;
  };
}

#endif
