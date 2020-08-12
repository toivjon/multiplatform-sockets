#include "mps/udp_socket.h"

using namespace mps;

UDPSocket::UDPSocket(AddressFamily addressFamily) : Socket(addressFamily, Protocol::UDP) {
}

UDPSocket& UDPSocket::operator=(UDPSocket&& other) noexcept {
  // TODO
  return *this;
}

UDPSocket::~UDPSocket() {
}
