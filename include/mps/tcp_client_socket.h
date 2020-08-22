#ifndef MPS_TCP_CLIENT_SOCKET_H
#define MPS_TCP_CLIENT_SOCKET_H

#include "tcp_socket.h"

#include <vector>

namespace mps {
  class TCPClientSocket : public TCPSocket {
  public:
    TCPClientSocket(const Address& address, SocketHandle handle);
    TCPClientSocket(const Address& address);
    TCPClientSocket(const Address& address, const std::set<Flag>& flags);

    virtual ~TCPClientSocket() = default;

    void send(const std::vector<Byte>& bytes);

    std::vector<Byte> recv();
    std::vector<Byte> recv(int maxDataSize);
  };
}

#endif
