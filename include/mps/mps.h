#ifndef MPS_H
#define MPS_H

#include <cstdint>

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

  #if _WIN32
  // Handle for underlying Winsock 2 socket handle.
  typedef SOCKET SocketHandle;
  #else
  // Handle for underlying UNIX socket handle.
  typedef int SocketHandle;
  #endif

  // The default maximum amount of bytes to receive on a receive call.
  constexpr auto DefaultMaxReceiveDataLength = 1024;
}

#endif
