#include "mps/socket.h"

#include <algorithm>

using namespace mps;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

Socket::Socket() : Socket(INVALID_SOCKET) {
}

Socket::Socket(SocketHandle handle) : mHandle(handle) {
}

Socket::Socket(AddressFamily af, SocketType type) : Socket() {
}

Socket::Socket(Socket&& rhs) noexcept : Socket() {
  std::swap(mHandle, rhs.mHandle);
}

Socket& Socket::operator=(Socket&& rhs) noexcept {
  std::swap(mHandle, rhs.mHandle);
  return *this;
}

Socket::~Socket() {
  // TODO close socket
}
