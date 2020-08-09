#include "mps/address.h"

#include <algorithm>

using namespace mps;

Address::Address() : mPort(DefaultPort) {
}

Address::Address(const Address& other) : mPort(other.mPort) {
}

Address::Address(Address&& other) noexcept {
  swap(other);
}

Address& Address::operator=(const Address& other) {
  mPort = other.mPort;
}

Address& Address::operator=(Address&& other) noexcept {
  swap(other);
}

Address::~Address() {
}

void Address::swap(Address& address) noexcept {
  std::swap(mPort, address.mPort);
}
