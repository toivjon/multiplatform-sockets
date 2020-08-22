#include "mps/udp_packet.h"

using namespace mps;

UDPPacket::UDPPacket() : UDPPacket(Address(), {}) {
}

UDPPacket::UDPPacket(const Address& address, const std::vector<Byte>& data)
  : mAddress(address), mData(data) {
}

UDPPacket::UDPPacket(const UDPPacket& rhs) {
  mAddress = rhs.mAddress;
  mData = rhs.mData;
}

UDPPacket::UDPPacket(UDPPacket&& rhs) noexcept {
  mAddress = std::move(rhs.mAddress);
  mData = std::move(rhs.mData);
}

UDPPacket& UDPPacket::operator=(const UDPPacket& rhs) {
  mAddress = rhs.mAddress;
  mData = rhs.mData;
  return *this;
}

UDPPacket& UDPPacket::operator=(UDPPacket&& rhs) noexcept {
  mAddress = std::move(rhs.mAddress);
  mData = std::move(rhs.mData);
  return *this;
}

const std::string UDPPacket::toString() const {
  // TODO check how this could be done in a more elegant way.
  // append the zero flag into buffer to indicate string end.
  std::vector<Byte> buffer(mData);
  buffer.push_back('\0');

  // TODO append address into the result.
  std::string result;
  result += reinterpret_cast<const char*>(&buffer[0]);
  return result;
}
