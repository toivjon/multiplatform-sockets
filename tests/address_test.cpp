#include "mps.h"
#include <gtest/gtest.h>

using namespace mps;

TEST(AddressTest, DefaultConstructor)
{
  auto addr = Address();
  ASSERT_EQ(AddressFamily::IPv4, addr.getFamily());
  ASSERT_EQ(0, addr.getPort());
}
