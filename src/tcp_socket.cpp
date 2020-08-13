#include "mps/tcp_socket.h"

using namespace mps;

#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
#else
// Unix based variants use Unix API for sockets.
#include <sys/types.h>
#include <sys/socket.h>
constexpr SocketHandle InvalidHandle = -1;
constexpr int SocketError = -1;
#endif

TCPSocket::TCPSocket(AddressFamily addressFamily) : Socket(addressFamily, Protocol::TCP) {
}

TCPSocket& TCPSocket::operator=(TCPSocket&& other) noexcept {
  // TODO
  return *this;
}

TCPSocket::~TCPSocket() {
}

void TCPSocket::listen(int backlog) {
  // TODO sanity check that socket has been bound?
  auto result = ::listen(mHandle, backlog);
  if (result == SocketError) {
    // TODO handle error by throwing an exception
  }
}

Address TCPSocket::accept() {
  // TODO sanity check that socket has been bound?
  sockaddr address = {};
  socklen_t addressSize = 0;
  auto result = ::accept(mHandle, &address, &addressSize);
  if (result == InvalidHandle) {
    // TODO handle error by throwing an exception
  }
  return Address(address, addressSize);
}
