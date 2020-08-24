#include "mps/udp_packet.h"

using namespace mps;

UDPPacket::UDPPacket() : UDPPacket(Address(), {}) {
}

UDPPacket::UDPPacket(const Address& address, const Bytes& data)
  : mAddress(address), mData(data) {
}

const std::string UDPPacket::toString() const {
  // TODO check how this could be done in a more elegant way.
  // append the zero flag into buffer to indicate string end.
  Bytes buffer(mData);
  buffer.push_back('\0');

  // TODO append address into the result.
  std::string result;
  result += reinterpret_cast<const char*>(&buffer[0]);
  return result;
}
