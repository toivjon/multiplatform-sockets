#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "address.h"
#include "address_family.h"
#include "protocol.h"

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
    Socket(AddressFamily addressFamily, Protocol protocol);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();

    // Swaps the encapsulated socket handle with other.
    void Swap(Socket& other) noexcept;

    void bind(const Address& address);

    AddressFamily getAddressFamily() const { return mAddressFamily; }
    Protocol getProtocol() const { return mProtocol; }
  private:
    SocketHandle  mHandle;
    AddressFamily mAddressFamily;
    Protocol      mProtocol;
  };
}

#endif
