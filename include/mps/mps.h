#ifndef MPS_H
#define MPS_H

#include <cstdint>
#include <vector>

// Windows and Xbox use Winsock 2 API for sockets.
#if _WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

namespace mps {
  // Port numbers belong into unsigned 16-bit number space.
  typedef uint16_t Port;
  // We support platforms where one byte contains 8 bits.
  typedef uint8_t Byte;
  // A type for a dynamic array of bytes.
  typedef std::vector<Byte> Bytes;

  #if _WIN32
  // Handle for underlying Winsock 2 socket handle.
  typedef SOCKET SocketHandle;
  #else
  // Handle for underlying UNIX socket handle.
  typedef int SocketHandle;
  #endif

  // The default maximum amount of bytes to receive on a receive call.
  constexpr auto DefaultMaxReceiveDataLength = 1024;

  // The default size for the listen backlog for TCP server sockets.
  constexpr int TCPBacklogSize = 4;

  // Address family defines whether address or socket handles IPv4 or IPv6.
  enum class AddressFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6
  };

  // Socket type defines whether socket handles UDP or TCP communication.
  enum class SocketType {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
  };

  // UDP flags used when sending data.
  enum class UDPSendFlag {
    DontRoute = MSG_DONTROUTE
  };

  // TCP flags used when sending data.
  enum class TCPSendFlag {
    DontRouter = MSG_DONTROUTE,
    OutOfBand = MSG_OOB
  };

  // UDP flags used when receiving data.
  enum class UDPReceiveFlag {
    Peek = MSG_PEEK,
    OutOfBand = MSG_OOB
  };

  // TCP flags used when receiving data.
  enum class TCPReceiveFlag {
    Peek = MSG_PEEK,
    OutOfBand = MSG_OOB
  };

  // A linger structure used to set and get linger definitions.
  struct Linger {
    bool enabled;
    int  timeoutSeconds;
  };
}

#endif
