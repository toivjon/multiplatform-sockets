#ifndef MPS_TCP_CLIENT_SOCKET_H
#define MPS_TCP_CLIENT_SOCKET_H

#include "address.h"
#include "tcp_socket.h"

namespace mps {
  class TCPClientSocket : public TCPSocket {
  public:
    TCPClientSocket(const Address& address, SocketHandle handle);
    TCPClientSocket(const Address& address);

    void send(const Bytes& bytes);

    Bytes receive();
    Bytes receive(int maxDataSize);

    const Address& getLocalAddress() const { return mLocalAddress; }
    const Address& getRemoteAddress() const { return mRemoteAddress; }
  private:
    Address mLocalAddress;
    Address mRemoteAddress;
  };
}

#endif
