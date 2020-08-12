#include "mps/address.h"
#include "mps/exception.h"

#include <algorithm>
#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include <ws2tcpip.h>
#else
// Unix based variants use Unix API for sockets.
#include <arpa/inet.h>
#endif

using namespace mps;

namespace {
  inline bool isIPv6Address(const std::string& ipAddress) {
    return ipAddress.find(":") != std::string::npos;
  }
}

Address::Address(const std::string& ipAddress, unsigned short port) : mSockAddr({}) {
  if (isIPv6Address(ipAddress)) {
    auto sockaddr = reinterpret_cast<sockaddr_in6*>(&mSockAddr);
    sockaddr->sin6_family = AF_INET6;
    sockaddr->sin6_port = htons(port);
    if (!inet_pton(AF_INET6, ipAddress.c_str(), &sockaddr->sin6_addr)) {
      throw InvalidAddressFormatException(ipAddress);
    }
  } else {
    auto sockaddr = reinterpret_cast<sockaddr_in*>(&mSockAddr);
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    if (!inet_pton(AF_INET, ipAddress.c_str(), &sockaddr->sin_addr)) {
      throw InvalidAddressFormatException(ipAddress);
    }
  }
}

Address::Address(const sockaddr& sockaddr, int sockaddrSize) {
  std::memcpy(&mSockAddr, &sockaddr, sockaddrSize);
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
  if (isIPv4()) {
    return sizeof(sockaddr_in);
  } else {
    return sizeof(sockaddr_in6);
  }
}

unsigned short Address::getPort() const {
  if (isIPv4()) {
    const auto& sockaddr = reinterpret_cast<const sockaddr_in*>(&mSockAddr);
    return ntohs(sockaddr->sin_port);
  } else {
    const auto& sockaddr = reinterpret_cast<const sockaddr_in6*>(&mSockAddr);
    return ntohs(sockaddr->sin6_port);
  }
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
