#include "mps/socket.h"
#include "mps/defines.h"

#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include "wsa.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

using namespace mps;

Socket::Socket() : mHandle(MPS_INVALID_HANDLE) {
  // WSA needs explicit initialization and shutdown.
  #if MPS_SOCKET_API == MPS_SOCKET_API_WSA
  static WSA wsa;
  #endif

  auto mHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mHandle == INVALID_SOCKET) {

  }

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
  if (mHandle != MPS_INVALID_HANDLE) {
    #if MPS_SOCKET_API == MPS_SOCKET_API_WSA
    closesocket(mHandle);
    #else
    close(mHandle);
    #endif
  }
}
