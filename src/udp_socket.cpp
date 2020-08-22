#include "mps/udp_socket.h"
#include "mps/exception.h"
#include "error.h"

#if defined(_WIN32)
#include "wsa.h"
constexpr int SocketError = SOCKET_ERROR;
typedef int socklen_t;
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
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

UDPSocket::UDPSocket(Port port, const std::set<Flag>& flags)
  : Socket(flags.find(Flag::IPv6) == flags.end() ? AddressFamily::IPv4 : AddressFamily::IPv6, SocketType::UDP) {
  auto isIPv6 = flags.find(Flag::IPv6) != flags.end();

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
