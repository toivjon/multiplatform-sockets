#include "mps.h"
#include "gtest/gtest.h"

using namespace mps;

TEST(UDPSocketTest, DefaultConstructor)
{
  UDPSocket socket;
  ASSERT_NE(0, socket.getLocalPort());
}
