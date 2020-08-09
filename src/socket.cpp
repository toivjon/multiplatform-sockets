#include "mps/socket.h"
#include "mps/defines.h"

using namespace mps;

Socket::Socket() : mHandle(MPS_INVALID_HANDLE) {
  #if MPS_SOCKET_API == MPS_SOCKET_API_WSA
  // TODO build WSA or/and increment ref counter
  #endif
  // TODO build socket
}

Socket::Socket(Socket&& other) noexcept : Socket() {
  // TODO
}

Socket& Socket::operator=(Socket&& other) noexcept {
  // TODO
  return *this;
}

Socket::~Socket() {
  // TODO close socket
  #if MPS_SOCKET_API == MPS_SOCKET_API_WSA
  // release WSA or decrement ref counter
  #endif
}
