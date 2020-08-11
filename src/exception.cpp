#include "mps/exception.h"

using namespace mps;

InvalidAddressFormatException::InvalidAddressFormatException(const std::string& address)
  : mAddress(address) {
  mMessage = "The string '" + address + "' is not a valid IPv4 or IPv6 address.";
}

const char* InvalidAddressFormatException::what() const noexcept {
  return mMessage.c_str();
}
