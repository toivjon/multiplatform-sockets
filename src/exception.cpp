#include "mps/exception.h"

using namespace mps;

InvalidAddressFormatException::InvalidAddressFormatException(const std::string& address)
  : mAddress(address) {
  mMessage = "The string '" + address + "' is not a valid IPv4 or IPv6 address.";
}

const char* InvalidAddressFormatException::what() const noexcept {
  return mMessage.c_str();
}

SocketException::SocketException(const std::string& operation, const std::string& message)
  : mMessage(operation + ": " + message) {
}

const char* SocketException::what() const noexcept {
  return mMessage.c_str();
}
