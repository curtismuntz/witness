#pragma once

#include <vector>
extern "C" {
#include "apriltag/apriltag.h"
#include "apriltag/apriltag_pose.h"
#include "apriltag/tag36h11.h"
#include "apriltag/tagStandard41h12.h"
}

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"

#include "witness/server/converters/converters.h"

namespace witness::server::vision {

class Detector {
 public:
  enum TagFamily {
    TAG36h11,
    TAG41h12,
  };
  Detector(double tag_size, double fx, double fy, double cx, double cy, TagFamily type);
  ~Detector();
  zarray_t *detect(const cv::Mat &mat);

  void draw(cv::Mat *frame, zarray_t *detections);

  std::vector<witness::server::converters::TagData> extract_detected_poses(zarray_t *detections);

 private:
  apriltag_family_t *tag_family_;
  apriltag_detector_t *tag_detector_;
  double tag_size_;
  double fx_;  // focal length in camera x-direction [px]
  double fy_;  // focal length in camera y-direction [px]
  double cx_;  // optical center x-coordinate [px]
  double cy_;  // optical center y-coordinate [px]
};

}  // namespace witness::server::vision
