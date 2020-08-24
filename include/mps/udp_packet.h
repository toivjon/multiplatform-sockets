#ifndef MPS_UDP_PACKET_H
#define MPS_UDP_PACKET_H

#include "address.h"
#include "mps/mps.h"

namespace mps {
  class UDPPacket {
  public:
    UDPPacket();
    UDPPacket(const Address& address, const Bytes& data);
    UDPPacket(const UDPPacket& rhs);
    UDPPacket(UDPPacket&& rhs) noexcept;

    UDPPacket& operator=(const UDPPacket& rhs);
    UDPPacket& operator=(UDPPacket&& rhs) noexcept;

    virtual ~UDPPacket() = default;

    // Set the address of the target receiver or sender.
    void setAddress(const Address& address) { mAddress = address; }

    // Get the address of the target receiver or sender.
    const Address& getAddress() const { return mAddress; }

    // Set the data that is associated with the packet.
    void setData(const Bytes& data) { mData = data; }

    // Get the data associated with the packet.
    const Bytes& getData() const { return mData; }

    // Get a string presentation of the packet.
    const std::string toString() const;
  private:
    Address mAddress;
    Bytes   mData;
  };
}

#endif
