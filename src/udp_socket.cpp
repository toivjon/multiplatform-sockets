#include "mps/udp_socket.h"
#include "mps/exception.h"
#include "error.h"

#if defined(_WIN32)
#include "wsa.h"
constexpr uint64_t InvalidHandle = INVALID_SOCKET;
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
constexpr uint64_t InvalidHandle = -1;
#endif

using namespace mps;

// Port number used to automatically select any available port.
constexpr Port AnyPort = 0;

UDPSocket::UDPSocket() : UDPSocket(AnyPort) {
}

UDPSocket::UDPSocket(Port port) : UDPSocket(port, {}) {
}

UDPSocket::UDPSocket(const std::set<Flag>& flags) : UDPSocket(AnyPort, flags) {
}

UDPSocket::UDPSocket(Port port, const std::set<Flag>& flags) : Socket() {
  auto isIPv6 = flags.find(Flag::IPv6) != flags.end();

  // TODO we should move this into more shared place where TCP socks can use this as well.
  // WSA needs explicit initialization and shutdown.
  #if _WIN32
  static WSA wsa;
  #endif

  // build a new UDP socket handle either for IPv4 or IPv6.
  mHandle = socket(isIPv6 ? AF_INET6 : AF_INET, SOCK_DGRAM, 0);
  if (mHandle == InvalidHandle) {
    throw SocketException("socket", GetErrorMessage());
  }

  // build an address descriptor and bind socket with the given instructions.
  mAddress = Address(isIPv6 ? AddressFamily::IPv6 : AddressFamily::IPv4, port);
  if (bind(mHandle, mAddress.asSockaddr(), (int)mAddress.getSize()) == SocketError) {
    throw SocketException("bind", GetErrorMessage());
  }

  // get the information about how the socket was bound.
  auto addrSize = static_cast<int>(mAddress.getSize());
  if (getsockname(mHandle, mAddress.asSockaddr(), &addrSize) == SocketError) {
    throw SocketException("getsockname", GetErrorMessage());
  }

  // enable UDP LAN broadcasting if the broadcast flag is set.
  auto value = (flags.find(Flag::Broadcast) == flags.end() ? '0' : '1');
  if (setsockopt(mHandle, SOL_SOCKET, SO_BROADCAST, &value, sizeof(value)) == SocketError) {
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

UDPSocket::UDPSocket(UDPSocket&& rhs) noexcept : Socket() {
  mHandle = std::move(rhs.mHandle);
  mAddress = std::move(rhs.mAddress);
}

UDPSocket& UDPSocket::operator=(UDPSocket&& rhs) noexcept {
  mHandle = std::move(rhs.mHandle);
  mAddress = std::move(rhs.mAddress);
  return *this;
}

UDPSocket::~UDPSocket() {
  if (mHandle != InvalidHandle) {
    #if _WIN32
    closesocket(mHandle);
    #else
    close(mHandle);
    #endif
  }
}

void UDPSocket::send(UDPPacket& packet) {
  const auto& addr = packet.getAddress();
  const auto& data = packet.getData();
  auto result = sendto(
    mHandle,
    reinterpret_cast<const char*>(&data[0]),
    static_cast<int>(data.size()),
    0,
    addr.asSockaddr(),
    static_cast<int>(addr.getSize()));
  if (result == SocketError) {
    throw SocketException("sendto", GetErrorMessage());
  }
}

UDPPacket UDPSocket::recv() {
  return recv(DefaultMaxReceiveDataLength);
}

UDPPacket UDPSocket::recv(int maxDataSize) {
  // reserve desired amount of memory for the incoming data and address.
  std::vector<Byte> bytes;
  bytes.resize(maxDataSize);
  sockaddr_storage address = {};
  socklen_t addressSize = sizeof(address);

  // take whatever awaits in the queue.
  auto result = recvfrom(
    mHandle,
    reinterpret_cast<char*>(&bytes[0]),
    maxDataSize,
    0,
    reinterpret_cast<sockaddr*>(&address),
    &addressSize);
  if (result == SocketError) {
    throw SocketException("recvfrom", GetErrorMessage());
  }

  // truncate bytes array and return results in a packet structure.
  bytes.resize(result);
  return UDPPacket(Address(address), bytes);
}
