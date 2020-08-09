#include "mps/socket.h"
#include "mps/defines.h"

#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include "wsa.h"
#endif

using namespace mps;

Socket::Socket() : mHandle(MPS_INVALID_HANDLE) {
  #if MPS_SOCKET_API == MPS_SOCKET_API_WSA
  static WSA wsa;
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
}
