#ifndef MPS_ADDRESS_H
#define MPS_ADDRESS_H

namespace mps {
  class Address {
  public:
    // The default port used when built with default constructor.
    static const unsigned short DefaultPort = 5555;

    Address();
    Address(const Address& other);
    Address(Address&& other) noexcept;

    Address& operator=(const Address& other);
    Address& operator=(Address&& other) noexcept;

    virtual ~Address();

    void swap(Address& address) noexcept;

    unsigned short getPort() const { return mPort; }
  private:
    unsigned short mPort;
  };
}

#endif
