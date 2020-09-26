#include "mps.h"
#include <gtest/gtest.h>

using namespace mps;

TEST(AddressExceptionTest, AddressParameterConstructor)
{
  auto address = "foobar";
  AddressException exception(address);
  ASSERT_EQ(address, exception.getAddress());
}
