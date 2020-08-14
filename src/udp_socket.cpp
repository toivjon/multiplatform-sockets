#include "mps/udp_socket.h"

#if defined(_WIN32)
constexpr int SocketError = SOCKET_ERROR;
#else
constexpr int SocketError = -1;
#endif

using namespace mps;

UDPSocket::UDPSocket(AddressFamily addressFamily) : Socket(addressFamily, Protocol::UDP) {
}

UDPSocket& UDPSocket::operator=(UDPSocket&& other) noexcept {
  // TODO
  return *this;
}

UDPSocket::~UDPSocket() {
}

void UDPSocket::sendTo(const Address& address, const void* data, size_t dataSize) {
  auto result = ::sendto(mHandle, data, dataSize, 0, address.asSockaddr(), address.getSize());
  if (result == SocketError) {
    // TODO handle error by throwing an exception 
  }
}

Address UDPSocket::recvFrom(void* data, size_t maxDataSize) {
  sockaddr_storage addr = {};
  socklen_t addrSize = 0;
  auto result = ::recvfrom(mHandle, data, maxDataSize, 0, reinterpret_cast<sockaddr*>(&addr), &addrSize);
  if (result == SocketError) {
    // TODO handle error by throwing an exception
  }
  return Address(addr);
}