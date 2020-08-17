#ifndef MPS_TCP_SOCKET_H
#define MPS_TCP_SOCKET_H

#include "address_family.h"
#include "socket.h"

namespace mps {
  class TCPSocket : public Socket {
  public:
    struct Connection {
      SocketHandle handle;
      Address      address;
    };
    TCPSocket(AddressFamily addressFamily);
    TCPSocket(SocketHandle handle, AddressFamily addressFamily);
    TCPSocket(const TCPSocket& other) = delete;
    TCPSocket(TCPSocket&& other) noexcept = default;

    TCPSocket& operator=(const TCPSocket& other) = delete;
    TCPSocket& operator=(TCPSocket&& other) noexcept;

    virtual ~TCPSocket();

    void listen(int backlog);
    Connection accept();
    void connect(const Address& address);
    void send(const void* data, size_t dataSize);
    void send(const std::string& message);
    int recv(void* data, size_t maxDataSize);
  };
}

#endif
