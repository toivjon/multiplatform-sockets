#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "address.h"
#include "mps.h"

#include <set>

namespace mps {
  class TCPSocket {
  public:
    enum class Flag { IPv6, NonBlock, NoNagle };

    TCPSocket(const Address& address, SocketHandle handle);
    TCPSocket(const Address& address, const std::set<Flag>& flags);
    TCPSocket(const TCPSocket& rhs) = delete;
    TCPSocket(TCPSocket&& rhs) noexcept = default;

    TCPSocket& operator=(const TCPSocket& rhs) = delete;
    TCPSocket& operator=(TCPSocket&& rhs) noexcept = default;

    virtual ~TCPSocket();

    // Get the details about the bound address.
    const Address& getAddress() const { return mAddress; }
  protected:
    SocketHandle  mHandle;
    Address       mAddress;
  };
}

#endif
