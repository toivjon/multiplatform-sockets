#ifndef MPS_ADDRESS_H
#define MPS_ADDRESS_H

#include <string>
#if defined(_WIN32)
// Windows and Xbox use Winsock 2 API for sockets.
#include <winsock2.h>
#else
// Unix based variants use Unix API for sockets.
#include <sys/types.h>
#include <sys/socket.h>
#endif

namespace mps {
  class Address {
  public:
    Address() = delete;
    Address(const std::string& ipAddress, unsigned short port);
    Address(const Address& other);
    Address(Address&& other) noexcept;

    Address& operator=(const Address& other);
    Address& operator=(Address&& other) noexcept;

    virtual ~Address();

    void swap(Address& address) noexcept;

    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

          sockaddr* asSockaddr()       { return reinterpret_cast<sockaddr*>(&mSockAddr);        }
    const sockaddr* asSockaddr() const { return reinterpret_cast<const sockaddr*>(&mSockAddr);  }

    size_t getSize() const;

    unsigned short getPort() const;

    std::string getIPAddress() const;
  protected:
    sockaddr_storage mSockAddr;
  };
}

#endif
