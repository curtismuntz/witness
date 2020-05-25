#include "witness/server/time/time.h"
#include "gtest/gtest.h"

namespace witness::server::time {
namespace {

TEST(Time, All) {
  ASSERT_TRUE(true);
  auto t1 = current_time();
  auto t2 = current_time();
  ASSERT_EQ(t1.first, t2.first);
  ASSERT_FALSE(t1.second == t2.second);
}

}  // namespace
}  // namespace witness::server::time

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
