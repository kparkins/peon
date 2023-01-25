#include <gtest/gtest.h>

#include "Pool.h"

struct Test {
  int x;
};
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
  // ChunkedPool<Test>* p = new ChunkedPool<Test>(3);
  // p->Size();
}

// int main(int argc, char** argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }