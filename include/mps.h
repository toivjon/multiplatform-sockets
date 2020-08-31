#ifndef MPS_H
#define MPS_H

#include <cstdint>
#include <exception>
#include <set>
#include <string>
#include <vector>

#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
constexpr auto CloseSocket = closesocket;
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
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
  // The size of the TCP server listen queue.
  constexpr int TCPServerListenQueueSize = 4;

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
    Address(const std::string& ip, uint16_t port) : mSockAddr({}) {
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
    Address(uint16_t port, AddressFamily af) : mSockAddr({}) {
      if (af == AddressFamily::IPv6) {
        auto sockaddr = reinterpret_cast<sockaddr_in6*>(&mSockAddr);
        sockaddr->sin6_family = AF_INET6;
        sockaddr->sin6_port = htons(port);
        sockaddr->sin6_addr = in6addr_any;
      } else {
        auto sockaddr = reinterpret_cast<sockaddr_in*>(&mSockAddr);
        sockaddr->sin_family = AF_INET;
        sockaddr->sin_port = htons(port);
        sockaddr->sin_addr.s_addr = INADDR_ANY;
      }
    }

    // Build a new address from the given socket address descriptor.
    Address(const sockaddr_storage& addr) : mSockAddr(addr) {}

    // Get the definition which tells whether the address presents IPv4 or IPv6 address.
    AddressFamily getFamily() const { return isIPv4() ? AddressFamily::IPv4 : AddressFamily::IPv6; }
    // Get the definition whether the address presents a IPv4 address.
    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    // Get the definition whether the address presents a IPv6 address.
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

    // Get a reference to the wrapped socket address as a sockaddr.
    operator sockaddr* () { return reinterpret_cast<sockaddr*>(&mSockAddr); }
    // Get a constant reference to the wrapped socket address as a sockaddr.
    operator const sockaddr* () const { return reinterpret_cast<const sockaddr*>(&mSockAddr); }
    // Get the size of the wrapped socket address structure.
    socklen_t getSize() const { return isIPv4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6); }

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
    AddressFamily getAddressFamily() const { return mLocalAddress.getFamily(); }
    // Get the definition whether the socket uses IPv4 address family.
    bool isIPv4() const { return mLocalAddress.getFamily() == AddressFamily::IPv4; }
    // Get the definition whether the socket uses IPv6 address family.
    bool isIPv6() const { return mLocalAddress.getFamily() == AddressFamily::IPv6; }

    // Specify the size for the incoming data buffer in bytes.
    void setReceiveBufferSize(int size) { setSockOpt(SOL_SOCKET, SO_RCVBUF, size); }
    // Specify the size for the outgoing data buffer in bytes.
    void setSendBufferSize(int size) { setSockOpt(SOL_SOCKET, SO_SNDBUF, size); }

    // Get the size of the incoming data buffer in bytes.
    int getReceiveBufferSize() const { return getSockOpt(SOL_SOCKET, SO_RCVBUF); }
    // Get the size of the outgoing data buffer in bytes.
    int getSendBufferSize() const { return getSockOpt(SOL_SOCKET, SO_SNDBUF); }

    // Specify whether the socket should route traffic or just directly use the interface.
    void setRouting(bool value) { setSockOpt(SOL_SOCKET, SO_DONTROUTE, value ? 1 : 0); }
    // Get the definition whether the socket routes traffic or directly uses the interface.
    bool isRouting() const { return getSockOpt(SOL_SOCKET, SO_DONTROUTE) == 0; }

    // Get the locally bound address of the socket.
    const Address& getLocalAddress() const { return mLocalAddress; }
    // Get the locally bound port of the socket.
    uint16_t getLocalPort() const { return mLocalAddress.getPort(); }
    // Get the locally bound IP of the socket.
    std::string getLocalIP() const { return mLocalAddress.getIP(); }

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
      if (fcntl(mHandle, F_SETFL, flags) == SOCKET_ERROR) {
        throw SocketException("fcntl(F_SETFL)");
      }
      #endif
      mBlocking = blocking;
    }
    // Get the definition whether socket operations are set to block or not.
    bool isBlocking() const { return mBlocking; }

  protected:
    Socket(AddressFamily af, int type) : mBlocking(true) {
      #if _WIN32
      static WinsockService winsock(2, 2);
      #endif
      mHandle = socket(static_cast<int>(af), type, 0);
      if (mHandle == INVALID_SOCKET) {
        throw SocketException("socket");
      }
    }

    // TODO check whether we should also pass blocking info here?
    Socket(SOCKET handle) : mHandle(handle), mBlocking(true) {
      refreshLocalAddress();
    }

    template<typename T>
    void setSockOpt(int level, int optKey, const T& value) {
      auto optVal = (const char*)&value;
      auto optLen = static_cast<int>(sizeof(T));
      if (setsockopt(mHandle, level, optKey, optVal, optLen) == SOCKET_ERROR) {
        throw SocketException("setsockopt(" + std::to_string(optKey) + ")");
      }
    }

    int getSockOpt(int level, int optKey) const {
      auto optVal = 0;
      #if _WIN32
      auto optLen = static_cast<int>(sizeof(int));
      #else
      auto optLen = static_cast<socklen_t>(sizeof(int));
      #endif
      if (getsockopt(mHandle, level, optKey, (char*)&optVal, &optLen) == SOCKET_ERROR) {
        throw SocketException("getsockopt(" + std::to_string(optKey) + ")");
      }
      return optVal;
    }

    void bind(const Address& address) {
      #if _WIN32
      auto addrSize = static_cast<int>(address.getSize());
      #else
      auto addrSize = static_cast<socklen_t>(address.getSize());
      #endif
      if (::bind(mHandle, address, addrSize) == SOCKET_ERROR) {
        throw SocketException("bind");
      }
      refreshLocalAddress();
    }

    void refreshLocalAddress() {
      #if _WIN32
      auto addrSize = static_cast<int>(sizeof(sockaddr_storage));
      #else
      auto addrSize = static_cast<socklen_t>(sizeof(sockaddr_storage));
      #endif
      if (getsockname(mHandle, mLocalAddress, &addrSize) == SOCKET_ERROR) {
        throw SocketException("getsockname");
      }
    }

    template<typename T>
    int buildFlagInt(const std::set<T>& flags) {
      int result = 0;
      for (T flag : flags) {
        result |= static_cast<int>(flag);
      }
      return result;
    }

    SOCKET        mHandle;
    bool          mBlocking;
    Address       mLocalAddress;
  private:
    #if _WIN32
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
    #endif

    void swap(Socket& rhs) noexcept {
      mHandle = rhs.mHandle;
      mBlocking = rhs.mBlocking;
      mLocalAddress = rhs.mLocalAddress;
      rhs.mHandle = INVALID_SOCKET;
    }
  };

  class TCPSocket : public Socket
  {
  public:
    // Specify whether the socket should use Nagle's algorithm to buffer data flow.
    void setBuffering(bool value) { setSockOpt(IPPROTO_TCP, TCP_NODELAY, value ? 0 : 1); }
    // Get the definition whether the socket uses Nagle's algorithm to buffer data flow.
    bool isBuffering() const { return getSockOpt(IPPROTO_TCP, TCP_NODELAY) == 0; }
  protected:
    // Build a new TCP socket with the given address family.
    TCPSocket(AddressFamily af) : Socket(af, SOCK_STREAM) {}
    // Build a new TCP socket with the given socket handle and with the given address family.
    TCPSocket(SOCKET handle) : Socket(handle) {}
  };

  // TCP flags used when sending data.
  enum class TCPSendFlag {
    DontRoute = MSG_DONTROUTE
  };

  // UDP flags used when receiving data.
  enum class TCPReceiveFlag {
    Peek = MSG_PEEK
  };

  class TCPClientSocket : public TCPSocket
  {
  public:
    // Construct a new TCP client by connecting to given server address.
    TCPClientSocket(const Address& addr) : TCPSocket(addr.getFamily()), mRemoteAddress(addr) {
      #if _WIN32
      auto addrSize = static_cast<int>(addr.getSize());
      #else
      auto addrSize = static_cast<socklen_t>(addr.getSize());
      #endif
      if (connect(mHandle, addr, addrSize) == SOCKET_ERROR) {
        throw SocketException("connect");
      }
      refreshLocalAddress();
    }
    // Build a new TCP client with the given socket handle and remote address.
    TCPClientSocket(SOCKET handle, const Address& addr) : TCPSocket(handle), mRemoteAddress(addr) {}

    // Get the remote address of the established TCP connection.
    const Address& getRemoteAddress() const { return mRemoteAddress; }
    // Get the port of the remote address of the established TCP connection.
    uint16_t getRemotePort() const { return mRemoteAddress.getPort(); }
    // Get the IP of the remote address of the established TCP connection.
    std::string getRemoteIP() const { return mRemoteAddress.getIP(); }

    // Specify whether the socket should send keep-alive messages to keep the connection alive.
    void setKeepAlive(bool value) { setSockOpt(SOL_SOCKET, SO_KEEPALIVE, value ? 1 : 0); }
    // Get the definition whether the socket keeps connection alive by sending keep-alive messages.
    bool isKeepAlive() const { return getSockOpt(SOL_SOCKET, SO_KEEPALIVE) == 1; }

    // Send the given bytes to connection destination with optional flags.
    void send(const Bytes& bytes, const std::set<TCPSendFlag>& flags = {}) {
      #if _WIN32
      auto data = reinterpret_cast<const char*>(&bytes[0]);
      auto size = static_cast<int>(bytes.size());
      #else
      auto data = reinterpret_cast<const void*>(&bytes[0]);
      auto size = bytes.size();
      #endif
      auto flag = buildFlagInt(flags);
      if (::send(mHandle, data, size, flag) == SOCKET_ERROR) {
        throw SocketException("send");
      }
    }

    // Receive incoming bytes from the connection.
    Bytes receive() { return receive(DefaultMaxReceiveDataSize, {}); }
    // Receive incoming bytes from the connection with the desired max data amount and flags.
    Bytes receive(int maxDataSize, const std::set<TCPReceiveFlag>& flags = {}) {
      Bytes bytes(maxDataSize);
      #if _WIN32
      auto data = reinterpret_cast<char*>(&bytes[0]);
      auto size = static_cast<int>(bytes.size());
      #else
      auto data = reinterpret_cast<void*>(&bytes[0]);
      auto size = bytes.size();
      #endif
      auto flag = buildFlagInt(flags);
      if (recv(mHandle, data, size, flag) == SOCKET_ERROR) {
        throw SocketException("recv");
      }
      return bytes;
    }
  private:
    Address mRemoteAddress;
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
    TCPServerSocket(const Address& address) : TCPSocket(address.getFamily()) {
      bind(address);
      if (listen(mHandle, TCPServerListenQueueSize) == SOCKET_ERROR) {
        throw SocketException("listen");
      }
    }

    // Accept the next incoming client connection.
    TCPClientSocket accept() {
      Address address;
      socklen_t addressSize = sizeof(sockaddr_storage);
      auto client = ::accept(mHandle, address, &addressSize);
      if (client == INVALID_SOCKET) {
        throw SocketException("accept");
      }
      return TCPClientSocket(client, address);
    }
  };

  struct UDPPacket
  {
    Address address; // The source/target address of the packet.
    Bytes   data;    // The data associated with the packet.
  };

  // UDP flags used when sending data.
  enum class UDPSendFlag {
    DontRoute = MSG_DONTROUTE
  };

  // UDP flags used when receiving data.
  enum class UDPReceiveFlag {
    Peek = MSG_PEEK
  };

  class UDPSocket : public Socket
  {
  public:
    // Build a new IPv4 socket with auto-selected port.
    UDPSocket() : UDPSocket(AnyPort) {};
    // Build a new IPv4 socket with the target port.
    UDPSocket(uint16_t port) : UDPSocket(port, AddressFamily::IPv4) {};
    // Build a new socket with the target address family and auto-selected port.
    UDPSocket(AddressFamily af) : UDPSocket(AnyPort, af) {}
    // Build a new socket with the target port and address family.
    UDPSocket(uint16_t port, AddressFamily af) : UDPSocket(Address(port, af)) {}
    // Build a new UDP socket and bind it to target address.
    UDPSocket(const Address& addr) : Socket(addr.getFamily(), SOCK_DGRAM) { bind(addr); }

    // Specify whether the socket can be used to broadcast packets in LAN.
    void setBroadcasting(bool value) { setSockOpt(SOL_SOCKET, SO_BROADCAST, value ? 1 : 0); }
    // Get the definition whether the socket can be used to broadcast packets in LAN.
    bool isBroadcasting() const { return getSockOpt(SOL_SOCKET, SO_BROADCAST) == 1; }

    // Send the data from the given packet into the target packet address with optional flags.
    void send(const UDPPacket& packet, const std::set<UDPSendFlag>& flags = {}) {
      #if _WIN32
      auto addrLen = static_cast<int>(packet.address.getSize());
      auto dataPtr = reinterpret_cast<const char*>(&packet.data[0]);
      auto dataLen = static_cast<int>(packet.data.size());
      #else
      auto addrLen = static_cast<socklen_t>(packet.address.getSize());
      auto dataPtr = reinterpret_cast<const void*>(&packet.data[0]);
      auto dataLen = packet.data.size();
      #endif
      auto flag = buildFlagInt(flags);
      if (sendto(mHandle, dataPtr, dataLen, flag, packet.address, addrLen) == SOCKET_ERROR) {
        throw SocketException("sendto");
      }
    }

    // Receive incoming data from the socket. Uses default value for the maximum data bytes.
    UDPPacket receive() { return receive(DefaultMaxReceiveDataSize, {}); }
    // Receive incoming data from the socket. Reads max of the given amount of bytes.
    UDPPacket receive(int maxDataSize, const std::set<UDPReceiveFlag>& flags = {}) {
      UDPPacket packet{ Address(), Bytes(maxDataSize) };
      #ifdef _WIN32
      auto addrLen = static_cast<int>(sizeof(sockaddr_storage));
      auto dataPtr = reinterpret_cast<char*>(&packet.data[0]);
      auto dataLen = static_cast<int>(packet.data.size());
      #else
      auto addrLen = static_cast<socklen_t>(sizeof(sockaddr_storage));
      auto dataPtr = reinteret_cast<void*>(&packet.data[0]);
      auto dataLen = packet.data.size();
      #endif
      auto flag = buildFlagInt(flags);
      auto result = recvfrom(mHandle, dataPtr, dataLen, flag, packet.address, &addrLen);
      if (result == SOCKET_ERROR) {
        throw SocketException("recvfrom");
      }
      packet.data.resize(result);
      return packet;
    }
  };

}

#endif
