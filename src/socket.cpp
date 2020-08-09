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
  Swap(other);
}

Socket& Socket::operator=(Socket&& other) noexcept {
  Swap(other);
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

void Socket::Swap(Socket& other) noexcept {
  auto oldHandle = mHandle;
  mHandle = other.mHandle;
  other.mHandle = oldHandle;
}
