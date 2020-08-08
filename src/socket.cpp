#include "mps/socket.h"

using namespace mps;

Socket::Socket() {
  // TODO build WSA or/and increment ref counter
  // TODO build socket
}

Socket::Socket(Socket&& other) noexcept {
  // TODO
}

Socket& Socket::operator=(Socket&& other) noexcept {
  // TODO
  return *this;
}

Socket::~Socket() {
  // TODO close socket
  // release WSA or decrement ref counter
}
