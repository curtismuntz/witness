#include "witness/server/vision/imshow_wrapper.h"
#include <opencv2/core/core.hpp>
#include "gtest/gtest.h"

namespace witness {
namespace server {
namespace vision {
namespace {

TEST(ImagesStartAtZero, All) {
  auto im = ImageManager();
  ASSERT_EQ(im.number_lookup("output"), 0);
}

TEST(ImagesIncreaseWithNewLookups, All) {
  auto im = ImageManager();
  ASSERT_EQ(im.number_lookup("output"), 0);
  ASSERT_EQ(im.number_lookup("output"), 1);
  ASSERT_EQ(im.number_lookup("output"), 2);
}

TEST(LookupsOnDifferentNamesDontAffectEachOther, All) {
  auto im = ImageManager();
  ASSERT_EQ(im.number_lookup("output1"), 0);
  ASSERT_EQ(im.number_lookup("output2"), 0);
  ASSERT_EQ(im.number_lookup("output1"), 1);
  ASSERT_EQ(im.number_lookup("output3"), 0);
  ASSERT_EQ(im.number_lookup("output2"), 1);
  ASSERT_EQ(im.number_lookup("output1"), 2);
}

}  // namespace
}  // namespace vision
}  // namespace server
}  // namespace witness

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
