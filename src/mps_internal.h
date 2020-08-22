#ifndef MPS_INTERNAL_H
#define MPS_INTERNAL_H

#include "mps/mps.h"

#if _WIN32
#include "wsa.h"
#else
#include <unistd.h>
#endif
#include <winsock2.h>

namespace mps {
  #if _WIN32
  // An invalid handle presents a unusable socket handle.
  constexpr SocketHandle InvalidHandle = INVALID_HANDLE;
  // Socket error presents a number indicating socket operation error.
  constexpr int SocketError = SOCKET_ERROR;
  #else
  // An invalid handle presents a unusable socket handle.
  constexpr SocketHandle InvalidHandle = -1;
  // Socket error presents a number indicating socket operation error.
  constexpr int SocketError = -1;
  #endif
}

#endif
