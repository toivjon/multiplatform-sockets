#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    TCPSocket(SocketHandle handle);
    TCPSocket(AddressFamily af);

    void setNagleEnabled(bool enabled);
    bool isNagleEnabled() const;
  };
}

#endif
