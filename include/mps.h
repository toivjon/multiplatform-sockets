#ifndef MPS_H
#define MPS_H

#include <cstdint>
#include <exception>
#include <string>

#if _WIN32
#include <winsock2.h>
constexpr auto CloseSocket = closesocket;
#else
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
  // A constant for specifying that the OS is free to select a port for the socket.
  constexpr uint16_t AnyPort = 0;

  // Address family defines whether address or socket handles IPv4 or IPv6.
  enum class AddressFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6
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
      DONT_ROUTE = SO_DONTROUTE
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

    AddressFamily mAddressFamily;
    SocketType    mType;
    SOCKET        mHandle;
    bool          mBlocking;
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

  class UDPSocket : public Socket
  {
  public:
    UDPSocket() : UDPSocket(AddressFamily::IPv4, AnyPort) {}
    UDPSocket(AddressFamily af, uint16_t port) : Socket(af, SocketType::UDP) {}
  };

  class TCPSocket : public Socket
  {
  public:
    TCPSocket(AddressFamily af) : Socket(af, SocketType::TCP) {}
  };

  }

#endif
