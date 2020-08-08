#include "mps/udp_socket.h"

using namespace mps;

UDPSocket::UDPSocket() : Socket() {
}

UDPSocket::UDPSocket(UDPSocket&& other) noexcept {
  // TODO
}

UDPSocket& UDPSocket::operator=(UDPSocket&& other) noexcept {
  // TODO
  return *this;
}

UDPSocket::~UDPSocket() {
}
