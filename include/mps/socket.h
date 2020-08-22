#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "mps.h"
#include "protocol.h"
#include "address_family.h"

namespace mps {
  class Socket {
  public:
    Socket();
    Socket(SocketHandle handle);
    Socket(AddressFamily af, Protocol protocol);
    Socket(const Socket&) = delete;
    Socket(Socket&& rhs) noexcept;

    virtual ~Socket();
  protected:
    SocketHandle mHandle;
  };
}

#endif
