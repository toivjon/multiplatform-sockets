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

TCPSocket::TCPSocket(SocketHandle handle) : Socket(handle), mNagleEnabled(true) {
}

TCPSocket::TCPSocket(AddressFamily af) : Socket(af, SocketType::TCP), mNagleEnabled(true) {
}

void TCPSocket::setNagleEnabled(bool enabled) {
  auto value = (enabled ? '0' : '1');
  if (setsockopt(mHandle, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) == SocketError) {
    throw SocketException("setsockopt", GetErrorMessage());
  }
}
