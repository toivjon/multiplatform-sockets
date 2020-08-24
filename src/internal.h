#ifndef MPS_INTERNAL_H
#define MPS_INTERNAL_H

#include "mps/mps.h"

namespace mps {
  // Port number used to automatically select any available port.
  constexpr Port AnyPort = 0;

  #if _WIN32
  constexpr int SocketError = SOCKET_ERROR;
  typedef int socklen_t;
  #else
  constexpr int SocketError = -1;
  #endif
}

#endif
