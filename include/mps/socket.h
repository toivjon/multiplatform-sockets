#ifndef MPS_SOCKET_H
#define MPS_SOCKET_H

namespace mps {
  class Socket {
  public:
    Socket();
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;

    virtual ~Socket();
  };
}

#endif
