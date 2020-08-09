#include "mps/socket.h"

#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
#else
// Unix based variants use Unix API for sockets.
#include <sys/types.h>
#include <sys/socket.h>
consexpr SocketHandle InvalidHandle = -1;
#endif

using namespace mps;

Socket::Socket() : mHandle(InvalidHandle) {
  // WSA needs explicit initialization and shutdown.
  #if defined(_WIN32)
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
  if (mHandle != InvalidHandle) {
    #if defined(_WIN32)
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
