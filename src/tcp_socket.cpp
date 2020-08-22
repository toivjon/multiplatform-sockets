#include "mps/tcp_socket.h"
#include "mps/exception.h"
#include "error.h"

#if _WIN32
#include "wsa.h"
constexpr uint64_t InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
#else
#include <unistd.h>
constexpr uint64_t InvalidHandle = -1;
constexpr int SocketError = -1;
#endif

using namespace mps;

// Port number used to automatically select any available port.
constexpr Port AnyPort = 0;

TCPSocket::TCPSocket(const Address& address, SocketHandle handle) : mAddress(address), mHandle(handle) {
}

TCPSocket::TCPSocket(const Address& address, const std::set<Flag>& flags) : mAddress(address) {
  auto isIPv6 = flags.find(Flag::IPv6) != flags.end();

  // TODO we should move this into more shared place where TCP socks can use this as well.
  // WSA needs explicit initialization and shutdown.
  #if _WIN32
  static WSA wsa;
  #endif

  // build a new TCP socket handle either for IPv4 or IPv6.
  mHandle = socket(isIPv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
  if (mHandle == InvalidHandle) {
    throw SocketException("socket", GetErrorMessage());
  }

  // disable Nagle's algorithm if the corresponding flag is set.
  auto value = (flags.find(Flag::NoNagle) == flags.end() ? '0' : '1');
  if (setsockopt(mHandle, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }

  #if _WIN32
  // enable non-blocking mode if the non-block flag has been set.
  auto isNonBlocking = flags.find(Flag::NonBlock) == flags.end() ? 0lu : 1lu;
  if (ioctlsocket(mHandle, FIONBIO, &isNonBlocking) == SocketError) {
    throw SocketException("ioctlsocket", GetErrorMessage());
  }
  #else
  // enable non-blocking mode if the non-block flag has been set.
  auto isBlocking = flags.find(Flag::NonBlock) == flags.end();
  auto flags = fcntl(mHandle, F_GETFL, 0);
  flags = isBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
  if (fcntl(mHandle, F_SETFL, flags) == SocketError) {
    throw SocketException("fcntl", GetErrorMessage());
  }
  #endif
}

TCPSocket::~TCPSocket() {
  if (mHandle != InvalidHandle) {
    #if _WIN32
    closesocket(mHandle);
    #else
    close(mHandle);
    #endif
  }
}
