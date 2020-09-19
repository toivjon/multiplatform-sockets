#include "mps.h"
#include "gtest/gtest.h"

using namespace mps;

TEST(example, foobar)
{
  auto addr = Address();
  ASSERT_EQ(AddressFamily::IPv4, addr.getFamily());
}
