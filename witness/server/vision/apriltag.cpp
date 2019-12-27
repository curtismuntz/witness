#include "witness/server/vision/apriltag.h"
#include "glog/logging.h"
#include "opencv2/imgproc.hpp"

namespace witness {
namespace server {
namespace vision {

Detector::Detector(double tag_size, double fx, double fy, double cx, double cy, TagFamily type)
    : tag_family_(NULL),
      tag_detector_(NULL),
      tag_size_(tag_size),
      fx_(fx),
      fy_(fy),
      cx_(cx),
      cy_(cy) {
  if (type == TAG36h11) {
    tag_family_ = tag36h11_create();
  } else if (type == TAG41h12) {
    tag_family_ = tagStandard41h12_create();
  }
  tag_detector_ = apriltag_detector_create();
  apriltag_detector_add_family(tag_detector_, tag_family_);

  // TODO(curtismuntz): configurables!
  tag_detector_->quad_decimate = 2.0;
  tag_detector_->quad_sigma = 0.0;
  tag_detector_->nthreads = 1;
  tag_detector_->debug = false;
  tag_detector_->refine_edges = true;
}

Detector::~Detector() {
  apriltag_detector_destroy(tag_detector_);
  tagStandard41h12_destroy(tag_family_);
}

zarray_t *Detector::detect(const cv::Mat &mat) {
  image_u8_t im = {.width = mat.cols, .height = mat.rows, .stride = mat.cols, .buf = mat.data};
  static zarray_t *detections;
  detections = apriltag_detector_detect(tag_detector_, &im);
  DLOG(INFO) << "Detected " << zarray_size(detections) << " tags.";
  return detections;
}

void Detector::draw(cv::Mat *frame, zarray_t *detections) {
  // Draw detection outlines
  for (int i = 0; i < zarray_size(detections); i++) {
    apriltag_detection_t *det;
    zarray_get(detections, i, &det);
    cv::line(*frame, cv::Point(det->p[0][0], det->p[0][1]), cv::Point(det->p[1][0], det->p[1][1]),
             cv::Scalar(0, 0xff, 0), 2);
    cv::line(*frame, cv::Point(det->p[0][0], det->p[0][1]), cv::Point(det->p[3][0], det->p[3][1]),
             cv::Scalar(0, 0, 0xff), 2);
    cv::line(*frame, cv::Point(det->p[1][0], det->p[1][1]), cv::Point(det->p[2][0], det->p[2][1]),
             cv::Scalar(0xff, 0, 0), 2);
    cv::line(*frame, cv::Point(det->p[2][0], det->p[2][1]), cv::Point(det->p[3][0], det->p[3][1]),
             cv::Scalar(0xff, 0, 0), 2);

    std::stringstream ss;
    ss << det->id;
    cv::String text = ss.str();
    int fontface = CV_FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontscale = 1.0;
    int baseline;
    cv::Size textsize = cv::getTextSize(text, fontface, fontscale, 2, &baseline);
    cv::putText(*frame, text,
                cv::Point(det->c[0] - textsize.width / 2, det->c[1] + textsize.height / 2),
                fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
  }
}

std::vector<witness::server::converters::TagData> Detector::extract_detected_poses(
    zarray_t *detections) {
  std::vector<witness::server::converters::TagData> poses;

  apriltag_detection_info_t info;
  info.tagsize = tag_size_;
  info.fx = fx_;
  info.fy = fy_;
  info.cx = cx_;
  info.cy = cy_;

  for (int i = 0; i < zarray_size(detections); i++) {
    apriltag_detection_t *det;
    zarray_get(detections, i, &det);
    info.det = det;

    apriltag_pose_t pose;
    auto tag = witness::server::converters::TagData();
    tag.id = det->id;
    tag.error = estimate_tag_pose(&info, &pose);
    tag.translation = witness::server::converters::get_translation(pose);
    tag.quaternion = witness::server::converters::get_quaternion(pose);
    poses.push_back(tag);
  }
  return poses;
}

}  // namespace vision
}  // namespace server
}  // namespace witness
