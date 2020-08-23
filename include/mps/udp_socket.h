#ifndef MPS_UDP_SOCKET_H
#define MPS_UDP_SOCKET_H

#include "address.h"
#include "socket.h"
#include "udp_packet.h"

#include <set>

namespace mps {
  class UDPSocket : public Socket {
  public:
    enum class Flag { IPv6 };

    UDPSocket();
    UDPSocket(Port port);
    UDPSocket(const std::set<Flag>& flags);
    UDPSocket(Port port, const std::set<Flag>& flags);
    UDPSocket(const UDPSocket&) = delete;
    UDPSocket(UDPSocket&& rhs) noexcept;

    UDPSocket& operator=(const UDPSocket&) = delete;
    UDPSocket& operator=(UDPSocket&& rhs) noexcept;

    virtual ~UDPSocket() = default;

    void send(UDPPacket& packet);

    UDPPacket recv();
    UDPPacket recv(int maxDataSize);

    // Get the details about the bound address.
    const Address& getAddress() const { return mAddress; }

    void setBroadcastEnabled(bool enabled);
    bool isBroadcastEnabled() const { return mBroadcastEnabled; }
  private:
    Address mAddress;
    bool    mBroadcastEnabled;
  };
}

#endif
