#ifndef MPS_EXCEPTION_H
#define MPS_EXCEPTION_H

#include <exception>
#include <string>

namespace mps {
  class InvalidAddressFormatException : std::exception {
  public:
    InvalidAddressFormatException(const std::string& address);
    const char* what() const noexcept override;
    const std::string& getAddress() const { return mAddress; }
  private:
    std::string mMessage;
    std::string mAddress;
  };

  class SocketException : std::exception {
  public:
    SocketException(const std::string& message);
    const char* what() const noexcept override;
  private:
    std::string mMessage;
  };
}

#endif
