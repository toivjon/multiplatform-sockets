#include "mps/address.h"

#include <algorithm>
#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

using namespace mps;

const std::string Address::DefaultHost = "127.0.0.1";

Address::Address() : Address(DefaultHost, DefaultPort) {
}

Address::Address(const std::string& host, unsigned short port) {
  mSockAddr = {}; // TODO
}

Address::Address(const Address& other) : mSockAddr(other.mSockAddr) {
}

Address::Address(Address&& other) noexcept {
  swap(other);
}

Address& Address::operator=(const Address& other) {
  mSockAddr = other.mSockAddr;
  return *this;
}

Address& Address::operator=(Address&& other) noexcept {
  swap(other);
  return *this;
}

Address::~Address() {
}

void Address::swap(Address& address) noexcept {
  std::swap(mSockAddr, address.mSockAddr);
}

size_t Address::getSize() const {
  return isIPv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
}

unsigned short Address::getPort() const {
  if (isIPv4()) {
    return reinterpret_cast<const sockaddr_in*>(&mSockAddr)->sin_port;
  }
  return reinterpret_cast<const sockaddr_in6*>(&mSockAddr)->sin6_port;
}
