#include "witness/server/file_operations.h"
#include "gtest/gtest.h"

namespace witness {
namespace server {
namespace file_operations {
namespace {

// TODO(curtismuntz) add more meaningful tests
TEST(ValidateExtensionTest, All) {
  ASSERT_FALSE(ValidateExtension("anyflag", "jpeg"));
  ASSERT_TRUE(ValidateExtension("anyflag", ".jpeg"));
}

}  // namespace
}  // namespace file_operations
}  // namespace server
}  // namespace witness

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
