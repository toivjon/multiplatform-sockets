#include "mps/tcp_socket.h"

using namespace mps;

TCPSocket::TCPSocket(AddressFamily addressFamily) : Socket(addressFamily, Protocol::TCP) {
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other) noexcept {
  // TODO
  return *this;
}

TCPSocket::~TCPSocket() {
}
