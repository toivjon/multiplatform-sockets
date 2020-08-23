#include "mps/tcp_socket.h"
#include "mps/exception.h"
#include "error.h"

#if _WIN32
constexpr int SocketError = SOCKET_ERROR;
#else
constexpr int SocketError = -1;
#endif

using namespace mps;

TCPSocket::TCPSocket(SocketHandle handle) : Socket(handle) {
}

TCPSocket::TCPSocket(AddressFamily af) : Socket(af, SocketType::TCP) {
}

void TCPSocket::setNagleEnabled(bool enabled) {
  int optVal = enabled ? 0 : 1;
  int optLen = sizeof(int);
  if (setsockopt(mHandle, IPPROTO_TCP, TCP_NODELAY, (const char*)&optVal, optLen) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }
}

bool TCPSocket::isNagleEnabled() const {
  int optVal = 0;
  int optLen = sizeof(int);
  if (getsockopt(mHandle, IPPROTO_TCP, TCP_NODELAY, (char*)&optVal, &optLen) == SocketError) {
    throw SocketException("getsockopt", GetErrorMessage());
  }
  return optVal == 0;
}
