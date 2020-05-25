#include "witness/server/webcam/actions/tracking.h"
#include "witness/server/converters/converters.h"
#include "witness/server/file_operations/file_operations.h"
#include "witness/server/vision/apriltag.h"
#include "witness/server/vision/imshow_wrapper.h"

extern "C" {
#include "apriltag/apriltag.h"
#include "apriltag/apriltag_pose.h"
#include "apriltag/tagStandard41h12.h"
}

namespace witness {
namespace server {
namespace webcam {
namespace actions {

Tracking::Tracking(std::shared_ptr<witness::webcam::Webcam> webcam,
                   const TrackingParameters &params, ServerWriter<StartAprilTrackingReply> *writer)
    : WebcamAction(webcam), params_(params), writer_(writer) {}

bool Tracking::Loop() {
  webcam_->OpenCamera();
  auto desired_filename = params_.fname_;
  cv::Mat gray;
  cv::Mat frame;
  int frame_number = 0;

  auto intrinsics = webcam_->GetCalibration();
  auto fx = intrinsics.at<double>(0, 0);
  auto fy = intrinsics.at<double>(1, 1);
  auto cx = intrinsics.at<double>(0, 2);
  auto cy = intrinsics.at<double>(1, 2);
  auto tagsize = 0.01;
  auto detector = witness::server::vision::Detector(
      tagsize, fx, fy, cx, cy, witness::server::vision::Detector::TagFamily::TAG36h11);
#ifdef IMSHOW_APRILTAGS
  auto imshow = witness::server::vision::ImageManager();
#endif
  while (working_ && webcam_->camera_->isOpened()) {
    webcam_->ReadGrey(&frame, &gray);
    auto detections = detector.detect(gray);
    auto num_detects = zarray_size(detections);
    if (num_detects > 0) {
      auto reply_msg = witness::api::StartAprilTrackingReply();
      detector.draw(&frame, detections);
      LOG(INFO) << num_detects << " detections.";
      auto tags = detector.extract_detected_poses(detections);

      for (auto i : tags) {
        witness::server::converters::add_tag(i, &reply_msg);
      }
      writer_->Write(reply_msg);
    }
#ifdef IMSHOW_APRILTAGS
    imshow.show("detections", frame);
#endif
    LOG(INFO) << "frame: " << frame_number;
    ++frame_number;
  }
  // video.release();
  webcam_->CloseCamera();
  return true;
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
