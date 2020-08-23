#include "mps/tcp_socket.h"
#include "mps/exception.h"
#include "error.h"

#if _WIN32
#include "wsa.h"
constexpr int SocketError = SOCKET_ERROR;
#else
#include <unistd.h>
constexpr int SocketError = -1;
#endif

using namespace mps;

// Port number used to automatically select any available port.
constexpr Port AnyPort = 0;

TCPSocket::TCPSocket(const Address& address, SocketHandle handle) : Socket(handle), mAddress(address), mNagleEnabled(true) {
}

TCPSocket::TCPSocket(const Address& address, const std::set<Flag>& flags)
  : Socket(flags.find(Flag::IPv6) == flags.end() ? AddressFamily::IPv4 : AddressFamily::IPv6, SocketType::TCP),
  mAddress(address), mNagleEnabled(true) {
}

void TCPSocket::setNagleEnabled(bool enabled) {
  auto value = (enabled ? '0' : '1');
  if (setsockopt(mHandle, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }
}
