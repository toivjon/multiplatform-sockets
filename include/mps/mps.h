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

  // A linger structure used to set and get linger definitions.
  struct Linger {
    bool enabled;
    int  timeoutSeconds;
  };
}

#endif
