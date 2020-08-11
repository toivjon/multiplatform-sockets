#include "mps/socket.h"

constexpr int DefaultProtocol = 0;
#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
#else
// Unix based variants use Unix API for sockets.
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
constexpr SocketHandle InvalidHandle = -1;
#endif

using namespace mps;

Socket::Socket(AddressFamily addressFamily) : mHandle(InvalidHandle) {
  // WSA needs explicit initialization and shutdown.
  #if defined(_WIN32)
  static WSA wsa;
  #endif

  auto domain = (addressFamily == AddressFamily::IPv4 ? AF_INET : AF_INET6);
  auto mHandle = socket(domain, SOCK_STREAM, DefaultProtocol);
  if (mHandle == InvalidHandle) {

  }

  // TODO build socket
}

Socket::Socket(Socket&& other) noexcept : Socket(AddressFamily::IPv4) { // TODO
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
