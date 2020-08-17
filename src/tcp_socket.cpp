#include "mps/tcp_socket.h"
#include "mps/exception.h"
#include "error.h"

using namespace mps;

#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
typedef char Data;
#else
// Unix based variants use Unix API for sockets.
#include <sys/types.h>
#include <sys/socket.h>
constexpr SocketHandle InvalidHandle = -1;
constexpr int SocketError = -1;
typedef void Data;
#endif

TCPSocket::TCPSocket(AddressFamily addressFamily) : Socket(addressFamily, Protocol::TCP) {
}

TCPSocket::TCPSocket(SocketHandle handle, AddressFamily addressFamily)
  : Socket(handle, addressFamily, Protocol::TCP) {
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
    throw SocketException("listen", GetErrorMessage());
  }
}

TCPSocket::Connection TCPSocket::accept() {
  // TODO sanity check that socket has been bound?
  sockaddr_storage address = {};
  socklen_t addressSize = sizeof(address);
  auto result = ::accept(mHandle, reinterpret_cast<sockaddr*>(&address), &addressSize);
  if (result == InvalidHandle) {
    throw SocketException("accept", GetErrorMessage());
  }
  Address addr(address);
  return { result, addr };
}

void TCPSocket::connect(const Address& address) {
  // TODO sanity check that socket is not yet connected?
  auto result = ::connect(mHandle, address.asSockaddr(), address.getSize());
  if (result == SocketError) {
    throw SocketException("connect", GetErrorMessage());
  }
}

void TCPSocket::send(const void* data, size_t dataSize) {
  // TODO sanity check that socket has been connected?
  auto result = ::send(mHandle, reinterpret_cast<const Data*>(data), dataSize, 0);
  if (result == SocketError) {
    throw SocketException("send", GetErrorMessage());
  }
}

void TCPSocket::send(const std::string& message) {
  send(message.c_str(), message.size());
}

int TCPSocket::recv(void* data, size_t maxDataSize) {
  // TODO sanity check that socket has been connected?
  auto result = ::recv(mHandle, reinterpret_cast<Data*>(data), maxDataSize, 0);
  if (result == SocketError) {
    throw SocketException("recv", GetErrorMessage());
  }
  return result;
}
