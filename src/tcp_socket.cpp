#include "mps/tcp_socket.h"
#include "mps/exception.h"

using namespace mps;

#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
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
    throw SocketException("Failed to start listen on socket"); // TODO
  }
}

Address TCPSocket::accept() {
  // TODO sanity check that socket has been bound?
  sockaddr address = {};
  socklen_t addressSize = 0;
  auto result = ::accept(mHandle, &address, &addressSize);
  if (result == InvalidHandle) {
    throw SocketException("Failed to accept incoming connection"); // TODO
  }
  return Address(address, addressSize);
}
