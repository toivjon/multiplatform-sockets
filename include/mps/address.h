#ifndef MPS_ADDRESS_H
#define MPS_ADDRESS_H

#include "defines.h"

#include <string>

namespace mps {
  class Address {
  public:
    // The default host used when built with default constructor.
    static const std::string DefaultHost;
    // The default port used when built with default constructor.
    static const unsigned short DefaultPort = 5555;

    Address();
    Address(const std::string& host, unsigned short port);
    Address(const Address& other);
    Address(Address&& other) noexcept;

    Address& operator=(const Address& other);
    Address& operator=(Address&& other) noexcept;

    virtual ~Address();

    void swap(Address& address) noexcept;

    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

    sockaddr* asSockaddr() { return reinterpret_cast<sockaddr*>(&mSockAddr); }

    size_t getSize() const;

    unsigned short getPort() const;
  protected:
    sockaddr_storage mSockAddr;
  };
}

#endif
