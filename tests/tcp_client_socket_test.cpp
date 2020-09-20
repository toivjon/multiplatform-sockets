#include "mps.h"
#include "gtest/gtest.h"

using namespace mps;

TEST(TCPClientSocketTest, AddressParameterConstructor)
{
  ASSERT_THROW(TCPClientSocket socket({ "127.0.0.1", 56789 }), SocketException);
}
