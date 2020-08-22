#ifndef MPS_UDP_PACKET_H
#define MPS_UDP_PACKET_H

#include "address.h"
#include "mps/mps.h"

#include <vector>

namespace mps {
  class UDPPacket {
  public:
    UDPPacket();
    UDPPacket(const Address& address, const std::vector<Byte>& data);
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
    void setData(const std::vector<Byte>& data) { mData = data; }

    // Get the data associated with the packet.
    const std::vector<Byte>& getData() const { return mData; }

    // Get a string presentation of the packet.
    const std::string toString() const;
  private:
    Address           mAddress;
    std::vector<Byte> mData;
  };
}

#endif
