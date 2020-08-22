#ifndef MPS_ADDRESS_H
#define MPS_ADDRESS_H

#include "mps.h"

#include <string>

namespace mps {
  class Address {
  public:
    Address();
    Address(AddressFamily addressFamily, unsigned short port);
    Address(const std::string& ipAddress, unsigned short port);
    Address(const sockaddr& sockaddr, int sockaddrSize);
    Address(const sockaddr_storage& sockaddr);
    Address(const Address& other);
    Address(Address&& other) noexcept;

    Address& operator=(const Address& other);
    Address& operator=(Address&& other) noexcept;

    virtual ~Address();

    // Swap the contents of the address with the target address.
    void swap(Address& address) noexcept;

    // Check whether the address is an IPv4 address.
    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    // Check whether the address is an IPv6 address.
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

    sockaddr* asSockaddr() { return reinterpret_cast<sockaddr*>(&mSockAddr); }
    const sockaddr* asSockaddr() const { return reinterpret_cast<const sockaddr*>(&mSockAddr); }

    // Get the size of the stored address definition (sockaddr).
    size_t getSize() const;

    // Get the port of the address.
    Port getPort() const;

    // Get the IP address of the address.
    std::string getIPAddress() const;

    AddressFamily getAddressFamily() const;
  protected:
    sockaddr_storage mSockAddr;
  };
}

#endif
