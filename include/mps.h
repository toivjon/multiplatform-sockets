#ifndef MPS_H
#define MPS_H

#include <cstdint>
#include <exception>
#include <set>
#include <string>
#include <vector>

#if _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace mps
{
  // Address family defines whether an address or socket handles IPv4 or IPv6.
  enum class AddressFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6
  };

  // An exception type thrown when the given address is not a valid IP address.
  class AddressException final : public std::exception
  {
  public:
    // Build a new address exception which tells that given address is invalid.
    AddressException(const std::string& address) : mAddress(address) {
      mMessage = "The '" + address + "' is not a valid IPv4 or IPv6 address.";
    }

    // Get the original address that was used when this exception was thrown.
    const std::string& getAddress() const { return mAddress; }

    // Print out what actually happened when this address exception was issued.
    const char* what() const noexcept override { return mMessage.c_str(); }
  private:
    std::string mAddress;
    std::string mMessage;
  };

  // A wrapper which is used to present a network address with an IP and port.
  class Address final
  {
  public:
    // Build a new IPv4 address with a undefined port and IP value definitions.
    Address() : Address(AddressFamily::IPv4, UndefinedPort) {
    }

    // Build a new address with given address family and undefined port and IP.
    Address(AddressFamily af) : Address(af, UndefinedPort) {
    }

    // Build a new undefined address with a specified port and address family.
    Address(AddressFamily af, uint16_t port) : mSockAddr({}) {
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

    // Build a new address with the specified port and IP address definitions.
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

    // Build a new address from the given socket address descriptor.
    Address(const sockaddr_storage& addr) : mSockAddr(addr) {
    }

    // Get the definition which tells whether the address presents IPv4 or IPv6 address.
    AddressFamily getFamily() const { return isIPv4() ? AddressFamily::IPv4 : AddressFamily::IPv6; }
    // Get the definition whether the address presents a IPv4 address.
    bool isIPv4() const { return mSockAddr.ss_family == AF_INET; }
    // Get the definition whether the address presents a IPv6 address.
    bool isIPv6() const { return mSockAddr.ss_family == AF_INET6; }

    // Get a reference to the wrapped socket address as a sockaddr.
    sockaddr* getSockaddr() { return reinterpret_cast<sockaddr*>(&mSockAddr); }
    // Get a constant reference to the wrapped socket address as a sockaddr.
    const sockaddr* getSockaddr() const { return reinterpret_cast<const sockaddr*>(&mSockAddr); }
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
    // An undefined port indicates that the address has not an assigned port.
    static auto const UndefinedPort = 0;

    // Check whether the given string contains IPv6 address.
    static bool isIPv6String(const std::string& val) {
      return val.find(":") != std::string::npos;
    }
    sockaddr_storage mSockAddr;
  };

  // An exception type thrown when logical errors arise from socket operations.
  class SocketException final : public std::exception
  {
  public:
    // Build an exception about the target operation and auto-resolved details.
    SocketException(const std::string& operation) : mOperation(operation) {
      #ifdef _WIN32
      auto flags = 0;
      flags |= FORMAT_MESSAGE_FROM_SYSTEM;     // we use WSAGetLastError() code
      flags |= FORMAT_MESSAGE_ALLOCATE_BUFFER; // auto-allocate the char buffer
      flags |= FORMAT_MESSAGE_IGNORE_INSERTS;  // let's ignore insert sequences
      LPSTR text(nullptr);
      auto error = WSAGetLastError();
      FormatMessageA(flags, nullptr, error, 0, (LPSTR)&text, 0, nullptr);
      if (text != nullptr) {
        mMessage += mOperation;
        mMessage += ": ";
        mMessage += std::string(text);
        LocalFree(text);
      }
      #else
      // TODO how to get perror message? check when testing in unix-environment
      #endif
    }

    // Get the name of the operation that was failed and caused this exception.
    const std::string& getOperation() const { return mOperation; }

    // Print out what actually happened when this socket exception was issued.
    const char* what() const noexcept override { return mMessage.c_str(); }
  private:
    std::string mOperation;
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
    virtual ~Socket() {
      #if _WIN32
      closesocket(mHandle);
      #else
      close(mHandle);
      #endif
    }

    // The definition which tells whether the socket handles IPv4 or IPv6 communication.
    AddressFamily getAddressFamily() const { return mLocalAddress.getFamily(); }
    // Get the definition whether the target socket uses IPv4 address family.
    bool isIPv4() const { return mLocalAddress.isIPv4(); }
    // Get the definition whether the target socket uses IPv6 address family.
    bool isIPv6() const { return mLocalAddress.isIPv6(); }

    // Specify the size of the incoming data buffer in bytes.
    void setReceiveBufferSize(int size) { setOpt(SOL_SOCKET, SO_RCVBUF, size); }
    // Specify the size of the outgoing data buffer in bytes.
    void setSendBufferSize(int size) { setOpt(SOL_SOCKET, SO_SNDBUF, size); }

    // Get the size of the incoming data buffer in bytes.
    int getReceiveBufferSize() const { return getOpt(SOL_SOCKET, SO_RCVBUF); }
    // Get the size of the outgoing data buffer in bytes.
    int getSendBufferSize() const { return getOpt(SOL_SOCKET, SO_SNDBUF); }

    // Specify whether the socket routes or directly uses the interface.
    void setRouting(bool value) { setOpt(SOL_SOCKET, SO_DONTROUTE, value); }
    // Get the definition whether the socket routes traffic or directly uses the interface.
    bool isRouting() const { return getOpt(SOL_SOCKET, SO_DONTROUTE) == 0; }

    // Get the locally bound address of the socket.
    const Address& getLocalAddress() const { return mLocalAddress; }
    // Get the locally bound port of the socket.
    uint16_t getLocalPort() const { return mLocalAddress.getPort(); }
    // Get the locally bound IP of the socket.
    std::string getLocalIP() const { return mLocalAddress.getIP(); }

    /// \brief Specify whether the socket should use blocking I/O operations.
    ///
    /// Block operation timeouts can be specified by corresponding timeout
    /// setters. Blocking TCP socket blocks with send and receive, while UDP
    /// sockets only block with receive function.
    ///
    /// \throws SocketException whether the configuration fails.
    ///
    /// \param blocking Definition to enable or disable blocking.
    /// 
    void setBlocking(bool blocking) {
      #if _WIN32
      auto flag = blocking ? 0lu : 1lu;
      if (ioctlsocket(mHandle, FIONBIO, &flag) == -1) {
        throw SocketException("ioctlsocket(FIONBIO)");
      }
      #else
      auto flags = fcntl(mHandle, F_GETFL, 0);
      flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
      if (fcntl(mHandle, F_SETFL, flags) == -1) {
        throw SocketException("fcntl(F_SETFL)");
      }
      #endif
      mBlocking = blocking;
    }

    /// \brief Check whether the socket currently uses blocking I/O operations.
    /// 
    /// Block operation timeouts can be specified by corresponding timeout
    /// setters. Blocking TCP socket blocks with send and receive, while UDP
    /// sockets only block with receive function.
    ///
    /// \throws SocketException whether the configuration query fails.
    /// 
    bool isBlocking() const {
      return mBlocking;
    }

    /// \brief Specify the maximum time to block when waiting for incoming I/O.
    ///
    /// Note that this definition only affects blocking sockets. Zero indicates
    /// that the receive operation will not time out.
    ///
    /// \throws SocketException whether the configuration fails.
    ///
    /// \param timeout The timeout in milliseconds.
    /// 
    void setReceiveTimeout(int timeout) {
      setOpt(SOL_SOCKET, SO_RCVTIMEO, timeout);
    }

    /// \brief Get the maximum time to block when waiting for incoming I/O.
    ///
    /// Note that this definition only affects blocking sockets. Zero indicates
    /// that the receive operation will not time out.
    ///
    /// \throws SocketException whether the configuration query fails.
    /// 
    int getReceiveTimeout() {
      return getOpt(SOL_SOCKET, SO_RCVTIMEO);
    }

  protected:
    #ifdef _WIN32
    static const auto InvalidSocket = INVALID_SOCKET;
    typedef SOCKET Handle;
    typedef char Data;
    typedef int DataSize;
    #else
    static const auto InvalidSocket = -1;
    typedef int Handle;
    typedef void Data;
    typedef size_t DataSize;
    #endif

    /// \brief Build a new socket with the given address family and socket type.
    ///
    /// This function is intended for internal use only and therefore it's being
    /// abstracted as a protected function. Function will also handle the static
    /// init of the winsockets when running on a Windows/Xbox platform.
    ///
    /// \throw SocketException when socket creation fails or if the Winsock init
    /// fails when running in a Windows environment.
    ///
    /// \param af The address family (IPv4 or IPv6) of the socket.
    /// \param type The type (Stream or Datagram) of the socket.
    /// 
    Socket(AddressFamily af, int type) : mBlocking(true) {
      #if _WIN32
      static WinsockService winsock(2, 2);
      #endif
      mHandle = socket(static_cast<int>(af), type, 0);
      if (mHandle == InvalidSocket) {
        throw SocketException("socket");
      }
    }

    /// \brief Build a socket with a socket handle and initial blocking state.
    ///
    /// This function is intended for internal use only and therefore its being
    /// abstracted as a protected function. Function is used to build a Socket
    /// wrapper around an already existing socket handle with known a blocking
    /// state. E.g. TCP server socket uses this to wrap new client connections.
    ///
    /// \throw SocketException whether the local address refresh fails.
    /// 
    /// \param sock The handle of the socket.
    /// \param blocking The definition whether the socket is in blocking state.
    /// 
    Socket(Handle sock, bool blocking) : mHandle(sock), mBlocking(blocking) {
      refreshLocalAddress();
    }

    /// \brief Retrieve and refresh the socket's local address information.
    ///
    /// This function will fetch the up-to-date information about the currently
    /// bound port and interface information about the socket. This function is
    /// being used after the socket has been bound or TCP server has accepted a
    /// new incoming client connection.
    ///
    /// \throw SocketException whether the local address refresh fails.
    /// 
    void refreshLocalAddress() {
      auto addrPtr = mLocalAddress.getSockaddr();
      auto addrLen = static_cast<socklen_t>(sizeof(sockaddr_storage));
      if (getsockname(mHandle, addrPtr, &addrLen) == -1) {
        throw SocketException("getsockname");
      }
    }

    /// \brief Bind the socket to the specified interface and port number.
    ///
    /// This function will bind the socket to the interface, socket and the
    /// address family type from the provided address encapsulation. After
    /// the operation is successfully finished, function will also refresh
    /// the local address information to contain the bound information.
    ///
    /// \throws SocketException whether the bind operation fails.
    ///
    /// \param address The address containing the target interface and port.
    /// 
    void bind(const Address& address) {
      if (::bind(mHandle, address.getSockaddr(), address.getSize()) == -1) {
        throw SocketException("bind");
      }
      refreshLocalAddress();
    }

    template<typename T>
    void setOpt(int level, int optKey, const T& value) {
      auto optVal = (const char*)&value;
      auto optLen = static_cast<socklen_t>(sizeof(T));
      if (setsockopt(mHandle, level, optKey, optVal, optLen) == -1) {
        throw SocketException("setsockopt(" + std::to_string(optKey) + ")");
      }
    }

    int getOpt(int level, int optKey) const {
      auto optVal = 0;
      auto optLen = static_cast<socklen_t>(sizeof(int));
      if (getsockopt(mHandle, level, optKey, (char*)&optVal, &optLen) == -1) {
        throw SocketException("getsockopt(" + std::to_string(optKey) + ")");
      }
      return optVal;
    }

    Handle        mHandle;
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
      rhs.mHandle = InvalidSocket;
    }
    bool          mBlocking;
    Address       mLocalAddress;
  };

  // An abstraction for all TCP sockets which cannot be directly constructed.
  class TCPSocket : public Socket
  {
  public:
    // Specify whether the socket uses Nagle's algorithm to buffer data flow.
    void setBuffering(bool value) { setOpt(IPPROTO_TCP, TCP_NODELAY, !value); }
    // Get whether the socket uses Nagle's algorithm to buffer data flow.
    bool isBuffering() const { return getOpt(IPPROTO_TCP, TCP_NODELAY) == 0; }
  protected:
    // Build a new TCP socket base instance with the specified address family.
    TCPSocket(AddressFamily af) : Socket(af, SOCK_STREAM) {
    }
    // Build a new TCP socket with the given socket handle and address family.
    TCPSocket(Handle sock, bool blocking) : Socket(sock, blocking) {
    }
  };

  class TCPClientSocket : public TCPSocket
  {
  public:
    // Construct a new TCP client by connecting to given server address.
    TCPClientSocket(const Address& addr) : TCPSocket(addr.getFamily()), mRemoteAddress(addr) {
      if (connect(mHandle, addr.getSockaddr(), addr.getSize()) == -1) {
        throw SocketException("connect");
      }
      refreshLocalAddress();
    }
    // Build a new TCP client with the given socket handle and remote address.
    TCPClientSocket(Handle handle, bool blocking, const Address& addr) : TCPSocket(handle, blocking), mRemoteAddress(addr) {
    }

    // Get the remote address of the established TCP connection.
    const Address& getRemoteAddress() const { return mRemoteAddress; }
    // Get the port of the remote address of the established TCP connection.
    uint16_t getRemotePort() const { return mRemoteAddress.getPort(); }
    // Get the IP of the remote address of the established TCP connection.
    std::string getRemoteIP() const { return mRemoteAddress.getIP(); }

    // Specify whether the socket should send keep-alive messages to keep the connection alive.
    void setKeepAlive(bool value) { setOpt(SOL_SOCKET, SO_KEEPALIVE, value); }
    // Get the definition whether the socket keeps connection alive by sending keep-alive messages.
    bool isKeepAlive() const { return getOpt(SOL_SOCKET, SO_KEEPALIVE) == 1; }

    /// \brief Specify the maximum time to block when waiting for outgoing I/O.
    ///
    /// Note that this definition only affects blocking sockets. Zero indicates
    /// that the send operation will not time out.
    ///
    /// \throws SocketException whether the configuration fails.
    ///
    /// \param timeout The timeout in milliseconds.
    /// 
    void setSendTimeout(int timeout) {
      setOpt(SOL_SOCKET, SO_SNDTIMEO, timeout);
    }

    /// \brief Get the maximum time to block when waiting for outgoing I/O.
    ///
    /// Note that this definition only affects blocking sockets. Zero indicates
    /// that the send operation will not time out.
    ///
    /// \throws SocketException whether the configuration query fails.
    /// 
    int getSendTimeout() {
      return getOpt(SOL_SOCKET, SO_SNDTIMEO);
    }

    // Send the given bytes to connection destination.
    void send(const std::vector<uint8_t>& bytes) {
      auto data = reinterpret_cast<const Data*>(&bytes[0]);
      auto size = static_cast<DataSize>(bytes.size());
      if (::send(mHandle, data, size, 0) == -1) {
        throw SocketException("send");
      }
    }

    // Receive incoming bytes from the connection with the desired max data amount.
    std::vector<uint8_t> receive(int maxDataSize = 1024) {
      std::vector<uint8_t> bytes(maxDataSize);
      auto data = reinterpret_cast<Data*>(&bytes[0]);
      auto size = static_cast<DataSize>(bytes.size());
      auto result = recv(mHandle, data, size, 0);
      if (result == -1) {
        throw SocketException("recv");
      }
      // TODO handle graceful close (result=0)
      return bytes;
    }
  private:
    Address mRemoteAddress;
  };

  class TCPServerSocket : public TCPSocket
  {
  public:
    /// \brief Build and auto-bind a TCP socket with the given address family.
    ///
    /// This function will build and bind a new TCP socket and assign it to all
    /// available local interfaces and allow the system to find a suitable port
    /// number for the socket.
    ///
    /// \throws SocketException if the bind or listen operation fails.
    ///
    /// \param af The definition whether socket uses IPv4 or IPv6.
    ///
    TCPServerSocket(AddressFamily af) : TCPServerSocket(af, 0) {
    }

    /// \brief Build and bind a TCP socket with given address family and port.
    ///
    /// This function will build and bind a new TCP socket and assign it to all
    /// available local interfaces and use the specified port number.
    ///
    /// \throws SocketException if the bind or listen operation fails.
    ///
    /// \param af The definition whether socket uses IPv4 or IPv6.
    /// \param port The port to be assigned for the socket.
    /// 
    TCPServerSocket(AddressFamily af, uint16_t port)
      : TCPServerSocket(Address(af, port)) {
    }

    /// \brief Build and bind a TCP socket with the given address definition.
    ///
    /// This function will build and bind a new TCP socket and assign it to the
    /// interface, port and IP address family from the given address object.
    ///
    /// \throws SocketException if the bind or listen operation fails.
    ///
    /// \param addr The address containing interface, port and address family.
    TCPServerSocket(const Address& addr) : TCPSocket(addr.getFamily()) {
      bind(addr);
      // TODO do we want to move this into separate function?
      if (listen(mHandle, 4) == -1) {
        throw SocketException("listen");
      }
    }

    // Accept the next incoming client connection.
    TCPClientSocket accept() {
      Address address;
      auto addrPtr = address.getSockaddr();
      socklen_t addressSize = sizeof(sockaddr_storage);
      auto client = ::accept(mHandle, addrPtr, &addressSize);
      if (client == InvalidSocket) {
        throw SocketException("accept");
      }
      return TCPClientSocket(client, isBlocking(), address);
    }
  };

  struct UDPPacket
  {
    Address               address; // The source/target address of the packet.
    std::vector<uint8_t>  data;    // The data associated with the packet.
  };

  class UDPSocket : public Socket
  {
  public:
    /// \brief Build and auto-bind a UDP socket with the given address family.
    ///
    /// This function will build and bind a new UDP socket and assign it to all
    /// available local interfaces and allow the system to find a suitable port
    /// number for the socket.
    ///
    /// \throws SocketException if the bind operation fails.
    ///
    /// \param af The definition whether the socket uses IPv4 or IPv6.
    /// 
    UDPSocket(AddressFamily af) : UDPSocket(af, UndefinedPort) {
    }

    /// \brief Build and bind a UDP socket with given address family and port.
    ///
    /// This function will build and bind a new UDP socket and assign it to all
    /// available local interfaces and use the specified port number.
    ///
    /// \throws SocketException if the bind operation fails.
    ///
    /// \param af The definition whether the socket uses IPv4 or IPv6.
    /// \param port The port to be assigned for the socket.
    UDPSocket(AddressFamily af, uint16_t port) : UDPSocket(Address(af, port)) {
    }

    /// \brief Build and bind a UDP socket with the given address definition.
    ///
    /// This function will build and bind a new UDP socket and assign it to the
    /// interface, port and IP address family from the given address object.
    ///
    /// \throws SocketException if the bind operation fails.
    ///
    /// \param addr The address containing interface, port and address family.
    /// 
    UDPSocket(const Address& addr) : Socket(addr.getFamily(), SOCK_DGRAM) {
      bind(addr);
    }

    /// \brief Specify whether the socket is allowed handle broadcast packets.
    ///
    /// Note that sockets do not automatically listen for broadcasted messages
    /// and therefore it's necessary to enable broadcasting at the sender and
    /// receiver side.
    ///
    /// \throws SocketException if the configuration fails.
    ///
    /// \param value The definition whether to enable or disable broadcasting.
    /// 
    void setBroadcasting(bool value) {
      setOpt(SOL_SOCKET, SO_BROADCAST, value ? 1 : 0);
    }

    /// \brief Check whether the socket is allowed to handle broadcast packets.
    ///
    /// Note that sockets do not automatically listen for broadcasted messages
    /// and therefore it's necessary to enable broadcasting at the sender and
    /// receiver side.
    ///
    /// \throws SocketException if the configuration querying fails.
    /// 
    bool isBroadcasting() const {
      return getOpt(SOL_SOCKET, SO_BROADCAST) == 1;
    }

    /// \brief Send or broadcast the provided packet to the target address.
    ///
    /// The provided packet contains the data to be sent along with remote
    /// host address and port number. If the message broadcasting have been
    /// enabled this function can be also used to send broadcast messages.
    ///
    /// \throws SocketException whether the send fails.
    ///
    /// \param packet The packet to be sent to target remote address.
    /// 
    void send(const UDPPacket& packet) {
      auto addrPtr = packet.address.getSockaddr();
      auto addrLen = packet.address.getSize();
      auto dataPtr = reinterpret_cast<const Data*>(&packet.data[0]);
      auto dataLen = static_cast<DataSize>(packet.data.size());
      if (sendto(mHandle, dataPtr, dataLen, 0, addrPtr, addrLen) == -1) {
        throw SocketException("sendto");
      }
    }

    // Receive incoming data from the socket. Reads max of the given amount of bytes.
    UDPPacket receive(int maxDataSize = 1024) {
      UDPPacket packet{ Address(), std::vector<uint8_t>(maxDataSize) };
      auto addrPtr = packet.address.getSockaddr();
      auto addrLen = static_cast<socklen_t>(sizeof(sockaddr_storage));
      auto dataPtr = reinterpret_cast<Data*>(&packet.data[0]);
      auto dataLen = static_cast<DataSize>(packet.data.size());
      auto result = recvfrom(mHandle, dataPtr, dataLen, 0, addrPtr, &addrLen);
      if (result == -1) {
        throw SocketException("recvfrom");
      }
      packet.data.resize(result);
      return packet;
    }
  private:
    // An undefined port indicates that the port can be auto-selected by OS.
    static auto const UndefinedPort = 0;
  };

}

#endif
