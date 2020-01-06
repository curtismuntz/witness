#include "witness/server/converters/converters.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace witness {
namespace server {
namespace converters {
namespace {

TEST(Conversions, All) {
  auto R = matd_identity(3);
  auto t = matd_create(3, 1);

  auto x = 1.1;
  auto y = 2.2;
  auto z = 3.3;

  t->data[0] = x;
  t->data[1] = y;
  t->data[2] = z;

  auto R00 = 1.1;
  auto R01 = 1.2;
  auto R02 = 1.3;
  auto R10 = 2.1;
  auto R11 = 2.2;
  auto R12 = 2.3;
  auto R20 = 3.1;
  auto R21 = 3.2;
  auto R22 = 3.3;

  R->data[0] = R00;
  R->data[1] = R01;
  R->data[2] = R02;
  R->data[3] = R10;
  R->data[4] = R11;
  R->data[5] = R12;
  R->data[6] = R20;
  R->data[7] = R21;
  R->data[8] = R22;

  // double vals[9] = {R00, R01, R02, R10, R11, R12, R20, R21, R22};
  // cv::Mat3d expected_mat(2, vals);
  Eigen::Matrix3d expected;
  expected << R00, R01, R02, R10, R11, R12, R20, R21, R22;

  apriltag_pose_t pose;
  pose.R = R;
  pose.t = t;

  auto quat = get_quaternion(pose);
  auto rot = get_rotation_matrix(pose);
  auto vec = get_translation(pose);
  LOG(INFO) << quat;
  LOG(INFO) << vec.transpose();

  ASSERT_EQ(vec.x(), x);
  ASSERT_EQ(vec.y(), y);
  ASSERT_EQ(vec.z(), z);
  ASSERT_EQ(rot, expected);

  matd_destroy(R);
  matd_destroy(t);
}

}  // namespace
}  // namespace converters
}  // namespace server
}  // namespace witness

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
