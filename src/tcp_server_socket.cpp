#include "mps/tcp_server_socket.h"
#include "mps/exception.h"
#include "error.h"

using namespace mps;

#if defined(_WIN32)
constexpr uint64_t InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
#else
constexpr uint64_t InvalidHandle = -1;
constexpr int SocketError = -1;
#endif

// Port number used to automatically select any available port.
constexpr Port AnyPort = 0;
// The default size for the listen backlog.
constexpr int BacklogSize = 4;

TCPServerSocket::TCPServerSocket() : TCPServerSocket(AnyPort, {}) {
}

TCPServerSocket::TCPServerSocket(Port port) : TCPServerSocket(port, {}) {
}

TCPServerSocket::TCPServerSocket(const Address& address) : TCPServerSocket(address, {}) {
}

TCPServerSocket::TCPServerSocket(Port port, const std::set<Flag>& flags)
  : TCPServerSocket(
    Address(flags.find(Flag::IPv6) != flags.end() ? AddressFamily::IPv6 : AddressFamily::IPv4, port),
    flags) {
}

TCPServerSocket::TCPServerSocket(const Address& address, const std::set<Flag>& flags)
  : TCPSocket(address, flags) {
  // bind the new socket with the given instructions.
  if (bind(mHandle, mAddress.asSockaddr(), (int)mAddress.getSize()) == SocketError) {
    throw SocketException("bind", GetErrorMessage());
  }

  if (listen(mHandle, BacklogSize) == SocketError) {
    throw SocketException("listen", GetErrorMessage());
  }
}

std::unique_ptr<TCPClientSocket> TCPServerSocket::accept() {
  sockaddr_storage address = {};
  socklen_t addressSize = sizeof(address);
  auto handle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&address), &addressSize);
  if (handle == InvalidHandle) {
    throw SocketException("accept", GetErrorMessage());
  }
  return std::make_unique<TCPClientSocket>(Address(address), handle);
}
