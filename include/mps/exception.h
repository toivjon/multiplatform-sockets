#ifndef MPS_EXCEPTION_H
#define MPS_EXCEPTION_H

#include <exception>
#include <string>

namespace mps {
  class InvalidAddressFormatException : std::exception {
  public:
    InvalidAddressFormatException(const std::string& address);
    const char* what() const override;
    const std::string& getAddress() const { return mAddress; }
  private:
    std::string mMessage;
    std::string mAddress;
  };
}

#endif
