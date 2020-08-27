#include "mps/tcp_server_socket.h"
#include "mps/exception.h"
#include "error.h"
#include "internal.h"

using namespace mps;

TCPServerSocket::TCPServerSocket() : TCPServerSocket(AnyPort, {}) {
}

TCPServerSocket::TCPServerSocket(Port port) : TCPServerSocket(port, AddressFamily::IPv4) {
}

TCPServerSocket::TCPServerSocket(Port port, AddressFamily af) : TCPServerSocket(Address(af, port)) {
}

TCPServerSocket::TCPServerSocket(const Address& address) : TCPSocket(address.getAddressFamily()) {
  // bind the new socket with the given instructions.
  if (bind(mHandle, address.asSockaddr(), (int)address.getSize()) == SocketError) {
    throw SocketException("bind", GetErrorMessage());
  }

  // get the information about how the socket was bound.
  auto addrSize = static_cast<int>(mLocalAddress.getSize());
  if (getsockname(mHandle, mLocalAddress.asSockaddr(), &addrSize) == SocketError) {
    throw SocketException("getsockname", GetErrorMessage());
  }

  if (listen(mHandle, TCPBacklogSize) == SocketError) {
    throw SocketException("listen", GetErrorMessage());
  }
}

std::unique_ptr<TCPClientSocket> TCPServerSocket::accept() {
  sockaddr_storage address = {};
  socklen_t addressSize = sizeof(address);
  auto handle = ::accept(mHandle, reinterpret_cast<sockaddr*>(&address), &addressSize);
  if (handle == SocketError) {
    throw SocketException("accept", GetErrorMessage());
  }
  return std::make_unique<TCPClientSocket>(Address(address), handle);
}
