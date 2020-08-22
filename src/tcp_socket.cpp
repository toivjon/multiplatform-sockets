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
