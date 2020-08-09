#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "mps/defines.h"

namespace mps {
  class Socket {
  public:
    Socket();
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();

    // Swaps the encapsulated socket handle with other.
    void Swap(Socket& other) noexcept;
  private:
    MPS_SOCKET_HANDLE mHandle;
  };
}

#endif
