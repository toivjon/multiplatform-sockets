#ifndef MPS_UDP_PACKET_H
#define MPS_UDP_PACKET_H

#include "address.h"

#include <cstdint>
#include <vector>

namespace mps {
  struct UDPPacket {
    Address               address;
    std::vector<uint8_t>  data;
  };
}

#endif
