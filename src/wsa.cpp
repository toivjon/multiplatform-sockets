#include "wsa.h"

#if MPS_SOCKET_API == MPS_SOCKET_API_WSA
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace mps;

WSA::WSA() {
  WSAData data;
  auto result = WSAStartup(MAKEWORD(2, 2), &data);
  if (result != 0) {
    // TODO throw exception based on the init error.
  }
}

WSA::~WSA() {
  WSACleanup();
}

#endif
