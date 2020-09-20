#include "mps.h"
#include "gtest/gtest.h"

using namespace mps;

TEST(TCPServerSocketTest, DefaultConstructor)
{
  TCPServerSocket socket(AddressFamily::IPv4);
  ASSERT_NE(0, socket.getLocalPort());
}
