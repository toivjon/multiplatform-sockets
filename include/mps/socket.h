#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "mps/defines.h"

#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include <winsock2.h>
#define MPS_SOCKET_HANDLE SOCKET
#else
#define MPS_SOCKET_HANDLE int
#endif

namespace mps {
  class Socket {
  public:
    Socket();
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();
  private:
    MPS_SOCKET_HANDLE mHandle;
  };
}

#endif
