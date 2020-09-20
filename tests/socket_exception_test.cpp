#include "mps.h"
#include "gtest/gtest.h"

using namespace mps;

TEST(SocketExceptionTest, OperationParameterConstructor)
{
  auto operation = "operation";
  SocketException exception(operation);
  ASSERT_EQ(operation, exception.getOperation());
}
