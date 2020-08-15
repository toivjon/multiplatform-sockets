#include "mps/udp_socket.h"
#include "mps/exception.h"

#if defined(_WIN32)
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
typedef char Data;
#else
constexpr int SocketError = -1;
typedef void Data;
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
  auto result = ::sendto(mHandle, reinterpret_cast<const Data*>(data), dataSize, 0, address.asSockaddr(), address.getSize());
  if (result == SocketError) {
    throw SocketException("Failed to send data via socket"); // TODO
  }
}

Address UDPSocket::recvFrom(void* data, size_t maxDataSize) {
  sockaddr_storage addr = {};
  socklen_t addrSize = 0;
  auto result = ::recvfrom(mHandle, reinterpret_cast<Data*>(data), maxDataSize, 0, reinterpret_cast<sockaddr*>(&addr), &addrSize);
  if (result == SocketError) {
    throw SocketException("Failed to receive data via socket"); // TODO
  }
  return Address(addr);
}
