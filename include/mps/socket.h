#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

#include "mps.h"

namespace mps {
  class Socket {
  public:
    Socket();
    Socket(SocketHandle handle);
    Socket(AddressFamily af, SocketType type);
    Socket(const Socket&) = delete;
    Socket(Socket&& rhs) noexcept = default;

    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&& rhs) noexcept = default;

    virtual ~Socket();

    void setNonBlocking(bool nonBlocking);
    bool isNonBlocking() const { return mNonBlocking; }

    void setReceiveBufferSize(int size);
    int getReceiveBufferSize() const;

    void setSendBufferSize(int size);
    int getSendBufferSize() const;

    void setNonRouting(bool nonRouting);
    bool isNonRouting() const;
  protected:
    SocketHandle  mHandle;
    bool          mNonBlocking;
  };
}

#endif
