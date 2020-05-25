#include <opencv2/core/core.hpp>
#include "witness/server/vision/apriltag.h"
#include "witness/server/file_operations/file_operations.h"
#include "gtest/gtest.h"
#include "witness/server/vision/imshow_wrapper.h"

extern "C" {
#include "apriltag/apriltag.h"
#include "apriltag/apriltag_pose.h"
#include "apriltag/tagStandard41h12.h"
}

#include "opencv2/highgui/highgui.hpp"

namespace witness::server::vision {
namespace {

TEST(Tag, All) {
  auto detector = Detector(0.01, 140.2, 140.4, 200.1, 200.0, Detector::TagFamily::TAG41h12);
  auto imshow = ImageManager();
  auto img = cv::imread("witness/server/vision/test_data/apriltag.jpg");
  cv::Mat gray_image;
  cv::cvtColor(img, gray_image, CV_BGR2GRAY);
  imshow.show("input", gray_image);

  auto detections = detector.detect(gray_image);
  auto num_detects = zarray_size(detections);
  ASSERT_EQ(num_detects, 1);

  detector.draw(&img, detections);
  imshow.show("annotated", img);
  auto poses_and_errors = detector.extract_detected_poses(detections);
  ASSERT_EQ(poses_and_errors.size(), 1);
  ASSERT_EQ(poses_and_errors[0].id, 5);
}

}  // namespace
}  // namespace witness::server::vision

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
