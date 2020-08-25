#include "mps/tcp_socket.h"
#include "mps/exception.h"
#include "error.h"
#include "internal.h"

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

void TCPSocket::setKeepAliveEnabled(bool enabled) {
  int optVal = enabled ? 1 : 0;
  int optLen = sizeof(int);
  if (setsockopt(mHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&optVal, optLen) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }
}

bool TCPSocket::isKeepAliveEnabled() const {
  int optVal = 0;
  int optLen = sizeof(int);
  if (getsockopt(mHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&optVal, &optLen) == SocketError) {
    throw SocketException("getsockopt", GetErrorMessage());
  }
  return optVal == 1;
}
