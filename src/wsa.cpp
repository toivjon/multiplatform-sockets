#include "wsa.h"

#if defined(_WIN32)
#include <winsock2.h>

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
