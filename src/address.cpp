#include "mps/address.h"
#include "mps/exception.h"

#include <algorithm>
#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

using namespace mps;

const std::string Address::DefaultHost = "127.0.0.1";

Address::Address() : Address(DefaultHost, DefaultPort) {
}

Address::Address(const std::string& ipAddress, unsigned short port) {
  mSockAddr = {};
  if (ipAddress.find(":") != std::string::npos) {
    // build as IPv6 address
    auto sockaddr = reinterpret_cast<sockaddr_in6*>(&mSockAddr);
    sockaddr->sin6_family = AF_INET6;
    sockaddr->sin6_port = htons(port);
    if (!inet_pton(AF_INET6, ipAddress.c_str(), &sockaddr->sin6_addr)) {
      throw InvalidAddressFormatException(ipAddress);

    }
  } else {
    // build as IPv4 address
    auto sockaddr = reinterpret_cast<sockaddr_in*>(&mSockAddr);
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    if (!inet_pton(AF_INET, ipAddress.c_str(), &sockaddr->sin_addr)) {
      throw InvalidAddressFormatException(ipAddress);
    }
  }
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
  return ntohs(isIPv4()
    ? reinterpret_cast<const sockaddr_in*>(&mSockAddr)->sin_port
    : reinterpret_cast<const sockaddr_in6*>(&mSockAddr)->sin6_port
  );
}

std::string Address::getIPAddress() const {
  if (isIPv4()) {
    char buffer[16];
    auto addr = reinterpret_cast<const sockaddr_in*>(&mSockAddr);
    inet_ntop(AF_INET, &addr->sin_addr, buffer, sizeof(buffer));
    return buffer;
  } else {
    char buffer[46];
    auto addr = reinterpret_cast<const sockaddr_in6*>(&mSockAddr);
    inet_ntop(AF_INET6, &addr->sin6_addr, buffer, sizeof(buffer));
    return buffer;
  }
}
