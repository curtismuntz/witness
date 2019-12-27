#include "witness/server/webcam/actions/monitor.h"

#include <vector>

namespace witness {
namespace server {
namespace webcam {
namespace actions {

Monitor::Monitor(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname)
    : WebcamAction(webcam), fname_(fname) {}

bool Monitor::MotionDetected(cv::Mat *foreground_mask) {
  int largest_area = 0;

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(*foreground_mask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

  for (size_t i = 0; i < contours.size(); i++) {
    double a = cv::contourArea(contours[i], false);
    if (a > largest_area) {
      largest_area = a;
    }
  }
  auto MAGIC_MOTION_THRESHOLD = 300;
  auto motion_detected = largest_area > MAGIC_MOTION_THRESHOLD ? true : false;

  return motion_detected;
}

bool Monitor::Loop() {
  webcam_->OpenCamera();

  cv::Mat foreground_mask;
  cv::Mat grey;
  cv::Mat frame;
  int frame_number = 0;

  cv::Ptr<cv::BackgroundSubtractor> pBackSub;
  pBackSub = cv::createBackgroundSubtractorMOG2();

  // seed background subtractor
  webcam_->ReadGrey(&frame, &grey);
  pBackSub->apply(grey, foreground_mask);

  auto video = webcam_->CreateVideoObject(fname_);

  LOG(INFO) << "STARTING MONITOR";
  while (working_ && webcam_->camera_->isOpened()) {
    webcam_->ReadGrey(&frame, &grey);
    pBackSub->apply(grey, foreground_mask);

    if (MotionDetected(&foreground_mask)) {
      LOG(INFO) << "MOTION DETECTED at frame " << frame_number;
      webcam_->WatermarkTime(&frame);
      video.write(frame);
    }
    ++frame_number;
  }

  video.release();
  webcam_->CloseCamera();
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
