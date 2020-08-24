#ifndef MPS_UDP_SOCKET_H
#define MPS_UDP_SOCKET_H

#include "address.h"
#include "socket.h"
#include "udp_packet.h"

namespace mps {
  class UDPSocket : public Socket {
  public:
    UDPSocket();
    UDPSocket(Port port);
    UDPSocket(AddressFamily af);
    UDPSocket(Port port, AddressFamily af);
    UDPSocket(UDPSocket&& rhs) noexcept;

    UDPSocket& operator=(UDPSocket&& rhs) noexcept;

    void send(UDPPacket& packet);

    UDPPacket receive();
    UDPPacket receive(int maxDataSize);

    // Get the details about the bound address.
    const Address& getAddress() const { return mAddress; }

    void setBroadcastEnabled(bool enabled);
    bool isBroadcastEnabled() const;
  private:
    Address mAddress;
  };
}

#endif
