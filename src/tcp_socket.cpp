#include "mps/tcp_socket.h"

using namespace mps;

TCPSocket::TCPSocket() : Socket() {
}

TCPSocket::TCPSocket(TCPSocket&& other) noexcept {
  // TODO
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other) noexcept {
  // TODO
  return *this;
}

TCPSocket::~TCPSocket() {
}
