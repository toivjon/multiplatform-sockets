#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "address.h"
#include "address_family.h"
#include "mps.h"
#include "protocol.h"

namespace mps {
  class Socket {
  public:
    Socket(SocketHandle handle, Address address);
    Socket(AddressFamily addressFamily, Protocol protocol);
    Socket(SocketHandle handle, AddressFamily addressFamily, Protocol protocol);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();

    // Swaps the encapsulated socket handle with other.
    void Swap(Socket& other) noexcept;

    void bind(const Address& address);
    void bind(Port port);

    AddressFamily getAddressFamily() const { return mAddressFamily; }
    Protocol getProtocol() const { return mProtocol; }

    void setBlocking(bool blocking);
    bool isBlocking() const { return mBlocking; }
  protected:
    SocketHandle  mHandle;
    AddressFamily mAddressFamily;
    Protocol      mProtocol;
    bool          mBlocking;
  };
}

#endif
