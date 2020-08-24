#ifndef MPS_INTERNAL_H
#define MPS_INTERNAL_H

#include "mps/mps.h"

namespace mps {
  #if _WIN32
  constexpr int SocketError = SOCKET_ERROR;
  typedef int socklen_t;
  #else
  constexpr int SocketError = -1;
  #endif
}

#endif
