#include "mps/socket.h"
#include "mps/exception.h"
#include "error.h"

#include <algorithm>

constexpr int DefaultProtocol = 0;
#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include "wsa.h"
constexpr SocketHandle InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
#else
// Unix based variants use Unix API for sockets.
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
constexpr SocketHandle InvalidHandle = -1;
constexpr int SocketError = -1;
#endif

using namespace mps;

Socket::Socket(AddressFamily addressFamily, Protocol protocol) :
  mHandle(InvalidHandle),
  mAddressFamily(addressFamily),
  mProtocol(protocol) {
  // WSA needs explicit initialization and shutdown.
  #if defined(_WIN32)
  static WSA wsa;
  #endif

  auto domain = (addressFamily == AddressFamily::IPv4 ? AF_INET : AF_INET6);
  auto type = (protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM);
  mHandle = socket(domain, type, DefaultProtocol);
  if (mHandle == InvalidHandle) {
    throw SocketException("socket", GetErrorMessage());
  }
}

Socket::Socket(SocketHandle handle, AddressFamily addressFamily, Protocol protocol) :
  mHandle(handle),
  mAddressFamily(addressFamily),
  mProtocol(protocol) {
}

Socket::Socket(Socket&& other) noexcept : Socket(AddressFamily::IPv4, Protocol::TCP) { // TODO
  Swap(other);
}

Socket& Socket::operator=(Socket&& other) noexcept {
  Swap(other);
  return *this;
}

Socket::~Socket() {
  if (mHandle != InvalidHandle) {
    #if defined(_WIN32)
    closesocket(mHandle);
    #else
    close(mHandle);
    #endif
  }
}

void Socket::Swap(Socket& other) noexcept {
  std::swap(mHandle, other.mHandle);
  std::swap(mAddressFamily, other.mAddressFamily);
  std::swap(mProtocol, other.mProtocol);
}

void Socket::bind(const Address& address) {
  // TODO some kind of sanity check so we're not rebinding this socket?
  auto result = ::bind(mHandle, address.asSockaddr(), static_cast<int>(address.getSize()));
  if (result == SocketError) {
    throw SocketException("bind", GetErrorMessage());
  }
}

void Socket::bind(uint16_t port) {
  bind(Address(mAddressFamily, port));
}
