#ifndef MPS_TCP_SERVER_SOCKET_H
#define MPS_TCP_SERVER_SOCKET_H

#include "tcp_client_socket.h"
#include "tcp_socket.h"

#include <memory>

namespace mps {
  class TCPServerSocket : public TCPSocket {
  public:
    TCPServerSocket();
    TCPServerSocket(Port port);
    TCPServerSocket(const Address& address);
    TCPServerSocket(Port port, const std::set<TCPSocket::Flag>& flags);
    TCPServerSocket(const Address& address, const std::set<Flag>& flags);

    virtual ~TCPServerSocket() = default;

    std::unique_ptr<TCPClientSocket> accept();
  };
}

#endif
