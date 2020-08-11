#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "address_family.h"

#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include <winsock2.h>
typedef SOCKET SocketHandle;
#else
// Unix based variants use Unix API for sockets.
typedef int SocketHandle;
#endif

namespace mps {
  class Socket {
  public:
    Socket(AddressFamily addressFamily);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();

    // Swaps the encapsulated socket handle with other.
    void Swap(Socket& other) noexcept;
  private:
    SocketHandle mHandle;
  };
}

#endif
