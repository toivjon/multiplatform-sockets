#include "mps/tcp_client_socket.h"
#include "mps/exception.h"
#include "error.h"
#include "internal.h"

using namespace mps;

TCPClientSocket::TCPClientSocket(const Address& address, SocketHandle handle) : TCPSocket(handle), mRemoteAddress(address) {
  auto addrSize = static_cast<int>(mLocalAddress.getSize());
  if (getsockname(mHandle, mLocalAddress.asSockaddr(), &addrSize) == SocketError) {
    throw SocketException("getsockname", GetErrorMessage());
  }
}

TCPClientSocket::TCPClientSocket(const Address& address) : TCPSocket(address.getAddressFamily()), mRemoteAddress(address) {
  auto result = ::connect(mHandle, address.asSockaddr(), static_cast<int>(address.getSize()));
  if (result == SocketError) {
    throw SocketException("connect", GetErrorMessage());
  }

  // get the information about how the socket was bound.
  auto addrSize = static_cast<int>(mLocalAddress.getSize());
  if (getsockname(mHandle, mLocalAddress.asSockaddr(), &addrSize) == SocketError) {
    throw SocketException("getsockname", GetErrorMessage());
  }
}

void TCPClientSocket::send(const Bytes& bytes) {
  auto result = ::send(
    mHandle,
    reinterpret_cast<const char*>(&bytes[0]),
    static_cast<int>(bytes.size()),
    0);
  if (result == SocketError) {
    throw SocketException("send", GetErrorMessage());
  }
}

Bytes TCPClientSocket::receive() {
  return receive(DefaultMaxReceiveDataLength);
}

Bytes TCPClientSocket::receive(int maxDataSize) {
  // reserve desired amount of memory for the incoming data and address.
  Bytes bytes;
  bytes.resize(maxDataSize);

  // take whatever awaits in the queue.
  auto result = recv(mHandle, reinterpret_cast<char*>(&bytes[0]), maxDataSize, 0);
  if (result == SocketError) {
    throw SocketException("recv", GetErrorMessage());
  }

  // truncate bytes array and return the results.
  bytes.resize(result);
  return bytes;
}
