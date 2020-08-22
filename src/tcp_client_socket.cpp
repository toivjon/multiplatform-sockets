#include "mps/tcp_client_socket.h"
#include "mps/exception.h"
#include "error.h"

using namespace mps;

#if _WIN32
constexpr int SocketError = SOCKET_ERROR;
#else
constexpr int SocketError = -1;
#endif

TCPClientSocket::TCPClientSocket(const Address& address, SocketHandle handle) : TCPSocket(address, handle) {
}

TCPClientSocket::TCPClientSocket(const Address& address) : TCPClientSocket(address, std::set<Flag>()) {
}

TCPClientSocket::TCPClientSocket(const Address& address, const std::set<Flag>& flags)
  : TCPSocket(
    Address(flags.find(Flag::IPv6) != flags.end() ? AddressFamily::IPv6 : AddressFamily::IPv4, 0),
    flags) {
  auto result = ::connect(mHandle, address.asSockaddr(), static_cast<int>(address.getSize()));
  if (result == SocketError) {
    throw SocketException("connect", GetErrorMessage());
  }
}

void TCPClientSocket::send(const std::vector<Byte>& bytes) {
  auto result = ::send(
    mHandle,
    reinterpret_cast<const char*>(&bytes[0]),
    static_cast<int>(bytes.size()),
    0);
  if (result == SocketError) {
    throw SocketException("send", GetErrorMessage());
  }
}

std::vector<Byte> TCPClientSocket::recv() {
  return recv(DefaultMaxReceiveDataLength);
}

std::vector<Byte> TCPClientSocket::recv(int maxDataSize) {
  // reserve desired amount of memory for the incoming data and address.
  std::vector<Byte> bytes;
  bytes.resize(maxDataSize);

  // take whatever awaits in the queue.
  auto result = ::recv(mHandle, reinterpret_cast<char*>(&bytes[0]), maxDataSize, 0);
  if (result == SocketError) {
    throw SocketException("recv", GetErrorMessage());
  }

  // truncate bytes array and return the results.
  bytes.resize(result);
  return bytes;
}
