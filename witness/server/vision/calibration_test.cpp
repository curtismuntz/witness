#include "witness/server/vision/calibration.h"
#include <opencv2/core/core.hpp>
#include "gtest/gtest.h"
#include "witness/server/file_operations/file_operations.h"

namespace witness::server::vision {
namespace {

bool matrix_equal(const cv::Mat &a, const cv::Mat &b) {
  return std::equal(a.begin<uchar>(), a.end<uchar>(), b.begin<uchar>());
}

TEST(BadParametersByDefault, All) {
  auto params = CalibrateParameters();
  ASSERT_FALSE(params.validate());
}

TEST(BadParameters, All) {
  auto params = CalibrateParameters();
  params.board_size_width = 0;
  params.board_size_height = 5;
  ASSERT_FALSE(params.validate());
}

TEST(GoodParameters, All) {
  auto params = CalibrateParameters();
  params.board_size_width = 5;
  params.board_size_height = 5;
  params.window_size = 11;
  params.minimum_calibration_samples = 25;
  ASSERT_TRUE(params.validate());
}

TEST(CalibratorReturnsUncalibrated, All) {
  auto params = CalibrateParameters();
  params.board_size_width = 5;
  params.board_size_height = 5;
  params.window_size = 11;
  params.minimum_calibration_samples = 25;

  auto calib = Calibrator(params);
  auto identity = cv::Mat::eye(3, 3, 1);
  auto camera = calib.get_camera_matrix();
  ASSERT_TRUE(matrix_equal(identity, camera));
}

// TODO(curtismuntz): Re-enable this test when a simple calibration data-set is committed.
// TEST(Calibrator, All) {
//   auto params = CalibrateParameters();
//   params.board_size_width = 8;
//   params.board_size_height = 6;
//   params.square_size_mm = 50;
//   params.window_size = 1;
//   params.minimum_calibration_samples = 8;
//   auto images =
//       witness::server::file_operations::ListDir("witness/server/vision/test_data", ".jpg", "noop");
//   // auto images =
//   // witness::server::file_operations::ListDir("witness/server/vision/test_data/opencv_data",
//   // ".jpg", "noop");
//
//   auto calib = Calibrator(params);
//   auto identity = cv::Mat::eye(3, 3, 1);
//   auto camera = calib.get_camera_matrix();
//   ASSERT_TRUE(matrix_equal(identity, camera));
//
//   for (auto i : images) {
//     calib.add_image_from_file(i);
//   }
//   auto result = calib.calibrate();
//   ASSERT_TRUE(result);
//   camera = calib.get_camera_matrix();
//   ASSERT_FALSE(matrix_equal(identity, camera));
//
//   // ASSERT_NE(calib.get_camera_matrix(), cv::Mat::eye(3,3,3));
// }

}  // namespace
}  // namespace witness::server::vision

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
