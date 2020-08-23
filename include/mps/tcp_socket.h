#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "address.h"
#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    TCPSocket(const Address& address, SocketHandle handle);
    TCPSocket(const Address& address, AddressFamily af);
    TCPSocket(const TCPSocket& rhs) = delete;
    TCPSocket(TCPSocket&& rhs) noexcept = default;

    TCPSocket& operator=(const TCPSocket& rhs) = delete;
    TCPSocket& operator=(TCPSocket&& rhs) noexcept = default;

    virtual ~TCPSocket() = default;

    // Get the details about the bound address.
    const Address& getAddress() const { return mAddress; }

    void setNagleEnabled(bool enabled);
    bool isNagleEnabled() const { return mNagleEnabled; }
  protected:
    Address mAddress;
    bool    mNagleEnabled;
  };
}

#endif
