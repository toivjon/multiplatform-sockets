# multiplatform-sockets
A simple multiplatform C++ wrapper for sockets.

TODO

- Add address option to check whether loopback.
  bool address_v4::is_loopback() const BOOST_ASIO_NOEXCEPT
  {
    return (to_uint() & 0xFF000000) == 0x7F000000;
  }
- Add address option to check whether undefined.
  bool address_v4::is_unspecified() const BOOST_ASIO_NOEXCEPT
  {
    return to_uint() == 0;
  }
- Add address option to check class.
  bool address_v4::is_class_a() const
  {
    return (to_uint() & 0x80000000) == 0;
  }
  bool address_v4::is_class_b() const
  {
    return (to_uint() & 0xC0000000) == 0x80000000;
  }
  bool address_v4::is_class_c() const
  {
    return (to_uint() & 0xE0000000) == 0xC0000000;
  }
- Add address option to check whether multicast address.
  bool address_v4::is_multicast() const BOOST_ASIO_NOEXCEPT
  {
    return (to_uint() & 0xF0000000) == 0xE0000000;
  }
- Add address support to get netmask.
  address_v4 address_v4::netmask(const address_v4& addr)
  {
    if (addr.is_class_a())
      return address_v4(0xFF000000);
    if (addr.is_class_b())
      return address_v4(0xFFFF0000);
    if (addr.is_class_c())
      return address_v4(0xFFFFFF00);
    return address_v4(0xFFFFFFFF);
  }
- Add address support to get broadcast address.
  address_v4 address_v4::broadcast(const address_v4& addr, const address_v4& mask)
  {
    return address_v4(addr.to_uint() | (mask.to_uint() ^ 0xFFFFFFFF));
  }
