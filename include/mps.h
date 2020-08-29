#ifndef MPS_H
#define MPS_H

#include <cstdint>
#include <exception>
#include <string>
#include <vector>

#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
constexpr auto CloseSocket = closesocket;
#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
typedef int SOCKET;
constexpr auto CloseSocket = close;
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

namespace mps
{
  // We support platforms where one byte contains 8 bits.
  typedef uint8_t Byte;
  // A type for a dynamic array of bytes.
  typedef std::vector<Byte> Bytes;

  // A constant for specifying that the OS is free to select a port for the socket.
  constexpr uint16_t AnyPort = 0;

  // A constant to specify the default maximum amount of incoming data to be read.
  constexpr int DefaultMaxReceiveDataSize = 1024;

  // Address family defines whether address or socket handles IPv4 or IPv6.
  enum class AddressFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6
  };

  // An exception type for logical exceptions that may arise from the address operations.
  class AddressException final : public std::exception
  {
  public:
    // Construct a new exception about the target address.
    AddressException(const std::string& address) : mAddress(address) {
      mMessage = "The '" + address + "' is not a valid IPv4 or IPv6 address.";
    }

    // Get the address that was related when this exception was issued.
    const std::string& getAddress() const { return mAddress; }

    // Print out what actually happened when this exception was issued.
    const char* what() const noexcept override { return mMessage.c_str(); }
  private:
    std::string mAddress;
    std::string mMessage;
  };

  // Address encapsulates the network IP address and the port.
  class Address final
  {
  public:
    // Construct a new IPv4 address with auto-select port definition.
    Address() : Address(AnyPort, AddressFamily::IPv4) {}

    // Construct a new address with the given port and IP address.
    Address(uint16_t port, const std::string& ip) {
      if (isIPv6String(ip)) {
        auto sockaddr = reinterpret_cast<sockaddr_in6*>(&mSockAddr);
        sockaddr->sin6_family = AF_INET6;
        sockaddr->sin6_port = htons(port);
        if (!inet_pton(AF_INET6, ip.c_str(), &sockaddr->sin6_addr)) {
          throw AddressException(ip);
        }
      } else {
        auto sockaddr = reinterpret_cast<sockaddr_in*>(&mSockAddr);
        sockaddr->sin_family = AF_INET;
        sockaddr->sin_port = htons(port);
        if (!inet_pton(AF_INET, ip.c_str(), &sockaddr->sin_addr)) {
          throw AddressException(ip);
        }
      }
    }

    // Construct a new address with the given port and any-interface.
    Address(uint16_t port, AddressFamily af) {
      if (af == AddressFamily::IPv6) {
        auto sockaddr = reinterpret_cast<sockaddr_in6*>(&mSockAddr);
        sockaddr->sin6_family = AF_INET6;
        sockaddr->sin6_port = htons(port);
        sockaddr->sin6_addr = in6addr_any;
      } else {
        auto sockaddr = reinterpret_cast<sockaddr_in*>(&mSockAddr);
        sockaddr->sin_family = AF_INET;
        sockaddr->sin_port = htons(port);
        sockaddr->sin_addr.S_un.S_addr = INADDR_ANY;
      }
    }

    // Build a new address from the given socket address descriptor.
    Address(const sockaddr_storage& addr) : mSockAddr(addr) {}

    // Get the definition which tells whether the address presents IPv4 or IPv6 address.
    AddressFamily getAddressFamily() const { return isIPv4() ? AddressFamily::IPv4 : AddressFamily::IPv6; }
    // Get the definition whether the address presents a IPv4 address.
    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    // Get the definition whether the address presents a IPv6 address.
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

    // Get a reference to the wrapped socket address as a sockaddr.
    sockaddr* asSockaddr() { return reinterpret_cast<sockaddr*>(&mSockAddr); }
    // Get a constant reference to the wrapped socket address as a sockaddr.
    const sockaddr* asSockaddr() const { return reinterpret_cast<const sockaddr*>(&mSockAddr); }
    // Get the size of the wrapped socket address structure.
    size_t getSize() const { return isIPv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6); }

    // Get the IP address of the address.
    std::string getIP() const {
      if (isIPv4()) {
        char buffer[16];
        auto addr = reinterpret_cast<const sockaddr_in*>(&mSockAddr);
        inet_ntop(AF_INET, &addr->sin_addr, buffer, sizeof(buffer));
        return buffer;
      } else {
        char buffer[46];
        auto addr = reinterpret_cast<const sockaddr_in6*>(&mSockAddr);
        inet_ntop(AF_INET6, &addr->sin6_addr, buffer, sizeof(buffer));
        return buffer;
      }
    }

    // Get the port of the address.
    uint16_t getPort() const {
      if (isIPv4()) {
        const auto& sockaddr = reinterpret_cast<const sockaddr_in*>(&mSockAddr);
        return ntohs(sockaddr->sin_port);
      } else {
        const auto& sockaddr = reinterpret_cast<const sockaddr_in6*>(&mSockAddr);
        return ntohs(sockaddr->sin6_port);
      }
    }
  private:
    // Check whether the given string contains IPv6 address.
    static bool isIPv6String(const std::string& val) {
      return val.find(":") != std::string::npos;
    }
    sockaddr_storage mSockAddr;
  };

  // An exception type for logical exceptions that may arise from the socket operations.
  class SocketException final : public std::exception
  {
  public:
    // Construct a new exception with the given operation name. Details are resolved automatically.
    SocketException(const std::string& operation) : mMessage(operation) {
      #ifdef _WIN32
      auto flags = 0;
      flags |= FORMAT_MESSAGE_FROM_SYSTEM;     // we want message for the WSAGetLastError() code
      flags |= FORMAT_MESSAGE_ALLOCATE_BUFFER; // allow formatter to allocate the char buffer
      flags |= FORMAT_MESSAGE_IGNORE_INSERTS;  // we don't want to use insert sequences
      LPSTR text(nullptr);
      if (FormatMessage(flags, nullptr, WSAGetLastError(), 0, (LPSTR)&text, 0, nullptr) != 0) {
        mMessage += ": ";
        mMessage += std::string(text);
        LocalFree(text);
      }
      #else

      // TODO how to get perror message? check when testing in unix-environment

      #endif
    }

    // Print out what actually happened when this exception was issued.
    const char* what() const noexcept override { return mMessage.c_str(); }
  private:
    std::string mMessage;
  };

  class Socket
  {
  public:
    // Copy semantics are disabled as we use RAII to handle our wrapped socket handle.
    Socket(const Socket& rhs) = delete;
    // Copy semantics are disabled as we use RAII to handle our wrapped socket handle.
    Socket& operator=(const Socket& rhs) = delete;

    // Move semantics are supported by using the swap functionality.
    Socket(Socket&& rhs) noexcept { swap(rhs); }
    // Move semantics are supported by using the swap functionality.
    Socket& operator=(Socket&& rhs) noexcept { swap(rhs); return *this; }

    // Destructor handles the automatic closing of the socket handle in a RAII way.
    virtual ~Socket() { CloseSocket(mHandle); }

    // The definition which tells whether the socket handles IPv4 or IPv6 communication.
    AddressFamily getAddressFamily() const { return mAddressFamily; }
    // Get the definition whether the socket uses IPv4 address family.
    bool isIPv4() const { return mAddressFamily == AddressFamily::IPv4; }
    // Get the definition whether the socket uses IPv6 address family.
    bool isIPv6() const { return mAddressFamily == AddressFamily::IPv6; }

    // Specify the size for the incoming data buffer in bytes.
    void setReceiveBufferSize(int size) { setSockOpt(SockOpt::RECEIVE_BUFFER_SIZE, size); }
    // Specify the size for the outgoing data buffer in bytes.
    void setSendBufferSize(int size) { setSockOpt(SockOpt::SEND_BUFFER_SIZE, size); }

    // Get the size of the incoming data buffer in bytes.
    int getReceiveBufferSize() const { return getSockOpt(SockOpt::RECEIVE_BUFFER_SIZE); }
    // Get the size of the outgoing data buffer in bytes.
    int getSendBufferSize() const { return getSockOpt(SockOpt::SEND_BUFFER_SIZE); }

    // Specify whether the socket should route traffic or just directly use the interface.
    void setRouting(bool value) { setSockOpt(SockOpt::DONT_ROUTE, value ? 1 : 0); }
    // Get the definition whether the socket routes traffic or directly uses the interface.
    bool isRouting() const { return getSockOpt(SockOpt::DONT_ROUTE) == 0; }

    // TODO getPort
    // TODO getIP
    // TODO getAddress

    // Specify whether the socket should block on blocking calls (e.g. receive etc.).
    void setBlocking(bool blocking) {
      #if _WIN32
      auto flag = blocking ? 0lu : 1lu;
      if (ioctlsocket(mHandle, FIONBIO, &flag) == SOCKET_ERROR) {
        throw SocketException("ioctlsocket(FIONBIO)");
      }
      #else
      auto flags = fcntl(mHandle, F_GETFL, 0);
      flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
      if (fcntl(mHandle, F_SETFL, flags) == SocketError) {
        throw SocketException("fcntl(F_SETFL)");
      }
      #endif
      mBlocking = blocking;
    }
    // Get the definition whether socket operations are set to block or not.
    bool isBlocking() const { return mBlocking; }

  protected:
    // Socket type defines whether socket handles UDP or TCP communication.
    enum class SocketType {
      TCP = SOCK_STREAM,
      UDP = SOCK_DGRAM
    };

    // Socket options can be used to customize the socket behaviour.
    enum class SockOpt {
      RECEIVE_BUFFER_SIZE = SO_RCVBUF,
      SEND_BUFFER_SIZE = SO_SNDBUF,
      DONT_ROUTE = SO_DONTROUTE,
      UDP_BROADCAST = SO_BROADCAST
    };

    Socket(AddressFamily af, SocketType type) : mAddressFamily(af), mType(type), mBlocking(true) {
      #if _WIN32
      static WinsockService winsock(2, 2);
      #endif
      mHandle = socket(static_cast<int>(af), static_cast<int>(type), 0);
      if (mHandle == INVALID_SOCKET) {
        throw SocketException("socket");
      }
    }

    template<typename T>
    void setSockOpt(SockOpt option, const T& value) {
      auto optKey = static_cast<int>(option);
      auto optVal = (const char*)&value;
      auto optLen = static_cast<int>(sizeof(T));
      if (setsockopt(mHandle, SOL_SOCKET, optKey, optVal, optLen) == SOCKET_ERROR) {
        throw SocketException("setsockopt(" + std::to_string(optKey) + ")");
      }
    }

    int getSockOpt(SockOpt option) const {
      auto optKey = static_cast<int>(option);
      auto optVal = 0;
      auto optLen = static_cast<int>(sizeof(int));
      if (getsockopt(mHandle, SOL_SOCKET, optKey, (char*)&optVal, &optLen) == SOCKET_ERROR) {
        throw SocketException("getsockopt(" + std::to_string(optKey) + ")");
      }
      return optVal;
    }

    void bind(const Address& address) {
      const auto& sockaddr = address.asSockaddr();
      const auto sockaddrSize = static_cast<int>(address.getSize());
      if (::bind(mHandle, sockaddr, sockaddrSize) == SOCKET_ERROR) {
        throw SocketException("bind");
      }
      refreshLocalAddress();
    }

    void refreshLocalAddress() {
      auto addrSize = static_cast<int>(mLocalAddress.getSize());
      if (getsockname(mHandle, mLocalAddress.asSockaddr(), &addrSize) == SOCKET_ERROR) {
        throw SocketException("getsockname");
      }
    }

    AddressFamily mAddressFamily;
    SocketType    mType;
    SOCKET        mHandle;
    bool          mBlocking;
    Address       mLocalAddress;
  private:
    // A RAII wrapper for proper initialization and graceful shutdown for the Winsock service.
    class WinsockService final
    {
    public:
      WinsockService(int verMajor, int verMinor) {
        if (WSAStartup(MAKEWORD(verMajor, verMinor), &mData) != 0) {
          throw SocketException("WSAStartup");
        }
      }
      ~WinsockService() {
        WSACleanup();
      }
    private:
      WSAData mData;
    };

    void swap(Socket& rhs) noexcept {
      mAddressFamily = rhs.mAddressFamily;
      mType = rhs.mType;
      mHandle = rhs.mHandle;
      mBlocking = rhs.mBlocking;
      rhs.mHandle = INVALID_SOCKET;
    }
  };

  class TCPSocket : public Socket
  {
  public:
    TCPSocket(AddressFamily af) : Socket(af, SocketType::TCP) {}


  };

  class TCPClientSocket : public TCPSocket
  {
  public:
    // Construct a new TCP client by connecting to given server address.
    TCPClientSocket(const Address& address) : TCPSocket(address.getAddressFamily()) {
      const auto& sockAddr = address.asSockaddr();
      auto sockAddrSize = static_cast<int>(address.getSize());
      if (connect(mHandle, sockAddr, sockAddrSize) == SOCKET_ERROR) {
        throw SocketException("connect");
      }
      refreshLocalAddress();
    }
    // TODO get remote address
    // TODO get remote port
    // TODO get remote ip

    // TODO send
    // TODO receive
    // TODO receive(maxDataSize)
    // TODO receive(bytes)
  };

  class TCPServerSocket : public TCPSocket
  {
  public:
    // Build a new IPv4 socket with auto-selected port and auto-selected interface.
    TCPServerSocket() : TCPServerSocket(AnyPort) {};
    // Build a new IPv4 socket with the target port in auto-selected interface.
    TCPServerSocket(uint16_t port) : TCPServerSocket(port, AddressFamily::IPv4) {}
    // Build a new socket with the target address family and port in the auto-selected interface.
    TCPServerSocket(uint16_t port, AddressFamily af) : TCPServerSocket(Address(port, af)) {}
    // Build a new TCP server socket and bind it to target address.
    TCPServerSocket(const Address& address) : TCPSocket(address.getAddressFamily()) {
      bind(address);
      // TODO specify listen with backlog size
    }
    // TODO accept
  };

  class UDPPacket
  {
  public:
    // Build a new UDP packet with empty address and bytes.
    UDPPacket() : UDPPacket(Address(), Bytes()) {}
    // Build a new UDP packet with given address and data.
    UDPPacket(const Address& address, const Bytes& data) : mAddress(address), mData(data) {}

    // Get the source / target address of the UDP packet.
    const Address& getAddress() const { return mAddress; }
    // Set the source / target address of the UDP packet.
    void setAddress(const Address& address) { mAddress = address; }

    // Get the data associated with the UDP packet.
    const Bytes& getData() const { return mData; }
    // Set the data associated with the UDP packet.
    void setData(const Bytes& data) { mData = data; }

    // Get the size of the packet data.
    size_t getSize() const { return mData.size(); }
  private:
    Address mAddress;
    Bytes   mData;
  };

  class UDPSocket : public Socket
  {
  public:
    // Build a new IPv4 socket with auto-selected port.
    UDPSocket() : UDPSocket(AnyPort) {};
    // Build a new IPv4 socket with the target port.
    UDPSocket(uint16_t port) : UDPSocket(port, AddressFamily::IPv4) {};
    // Build a new socket with the target port and address family.
    UDPSocket(uint16_t port, AddressFamily af) : UDPSocket(Address(port, af)) {}
    // Build a new UDP socket and bind it to target address.
    UDPSocket(const Address& address) : Socket(address.getAddressFamily(), SocketType::UDP) {
      bind(address);
    }

    // Specify whether the socket can be used to broadcast packets in LAN.
    void setBroadcasting(bool value) { setSockOpt(SockOpt::UDP_BROADCAST, value ? 1 : 0); }
    // Get the definition whether the socket can be used to broadcast packets in LAN.
    bool isBroadcasting() const { return getSockOpt(SockOpt::UDP_BROADCAST) == 1; }

    // TODO add a support for flags?
    // TODO add better functions to get required references
    // TODO improve the way how to indicate if connection was closed?
    void send(const UDPPacket& packet) {
      const auto& addr = packet.getAddress();
      const auto& data = reinterpret_cast<const char*>(&packet.getData()[0]);
      auto dataSize = static_cast<int>(packet.getSize());
      auto addrSize = static_cast<int>(addr.getSize());
      if (sendto(mHandle, data, dataSize, 0, addr.asSockaddr(), addrSize) == SOCKET_ERROR) {
        throw SocketException("sendto");
      }
    }

    // Receive incoming data from the socket. Uses default value for the maximum data bytes.
    UDPPacket receive() { return receive(DefaultMaxReceiveDataSize); }

    // TODO add a support for flags?
    // TODO add better functions to get required references
    // TODO improve the way how to indicate if connection was closed?
    // Receive incoming data from the socket. Reads max of the given amount of bytes.
    UDPPacket receive(int maxDataSize) {
      // reserve desired amount of memory for the incoming data and address.
      Bytes bytes(maxDataSize);
      sockaddr_storage address = {};
      socklen_t addrSize = sizeof(address);

      // take whatever awaits in the incoming queue.
      auto data = reinterpret_cast<char*>(&bytes[0]);
      auto addr = reinterpret_cast<sockaddr*>(&address);
      auto result = recvfrom(mHandle, data, maxDataSize, 0, addr, &addrSize);
      if (result == SOCKET_ERROR) {
        throw SocketException("recvfrom");
      }

      // truncate bytes array and return results in the package structure.
      bytes.resize(result);
      return UDPPacket(Address(address), bytes);
    }
  };

}

#endif
