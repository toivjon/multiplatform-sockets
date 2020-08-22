#include "mps/socket.h"

#include <algorithm>

using namespace mps;

#if _WIN32
constexpr uint64_t InvalidHandle = INVALID_SOCKET;
#else
constexpr uint64_t InvalidHandle = -1;
#endif

Socket::Socket() : Socket(InvalidHandle) {
}

Socket::Socket(SocketHandle handle) : mHandle(handle) {
}

Socket::Socket(AddressFamily af, Protocol protocol) : Socket() {
}

Socket::Socket(Socket&& rhs) noexcept : Socket() {
  std::swap(mHandle, rhs.mHandle);
}

Socket::~Socket() {
  // TODO close socket
}
