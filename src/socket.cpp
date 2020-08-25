#include "mps/socket.h"
#include "mps/exception.h"
#include "error.h"
#include "wsa.h"
#include "internal.h"

#if _WIN32
#define STATIC_INIT static WSA wsa
#define closesocket closesocket
#else
#define STATIC_INIT
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

using namespace mps;

Socket::Socket() : Socket(INVALID_SOCKET) {
}

Socket::Socket(SocketHandle handle) : mHandle(handle), mNonBlocking(false) {
}

Socket::Socket(AddressFamily af, SocketType type) : Socket() {
  STATIC_INIT;
  mHandle = socket(static_cast<int>(af), static_cast<int>(type), 0);
  if (mHandle == INVALID_SOCKET) {
    throw SocketException("socket", GetErrorMessage());
  }
}

Socket::~Socket() {
  if (mHandle != INVALID_SOCKET) {
    closesocket(mHandle);
  }
}

void Socket::setNonBlocking(bool nonBlocking) {
  #if _WIN32
  // enable non-blocking mode if the non-block flag has been set.
  auto flag = nonBlocking ? 1lu : 0lu;
  if (ioctlsocket(mHandle, FIONBIO, &flag) == SOCKET_ERROR) {
    throw SocketException("ioctlsocket", GetErrorMessage());
  }
  #else
  // enable non-blocking mode if the non-block flag has been set.
  auto flags = fcntl(mHandle, F_GETFL, 0);
  flags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  if (fcntl(mHandle, F_SETFL, flags) == SocketError) {
    throw SocketException("fcntl", GetErrorMessage());
  }
  #endif
  mNonBlocking = nonBlocking;
}

void Socket::setReceiveBufferSize(int size) {
  int optLen = sizeof(int);
  if (setsockopt(mHandle, SOL_SOCKET, SO_RCVBUF, (const char*)&size, optLen) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }
}

int Socket::getReceiveBufferSize() const {
  int optVal = 0;
  int optLen = sizeof(int);
  if (getsockopt(mHandle, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, &optLen) == SocketError) {
    throw SocketException("getsockopt", GetErrorMessage());
  }
  return optVal;
}
