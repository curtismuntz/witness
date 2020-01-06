#include "witness/server/webcam/actions/video_recorder.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

VideoRecorder::VideoRecorder(std::shared_ptr<witness::webcam::Webcam> webcam,
                             const std::string &fname)
    : WebcamAction(webcam), fname_(fname) {}

bool VideoRecorder::Loop() {
  webcam_->OpenCamera();

  auto video = webcam_->CreateVideoObject(fname_);

  cv::Mat frame;
  while (working_ && webcam_->camera_->isOpened()) {
    // get a new frame from camera
    if (!webcam_->ReadFrame(&frame)) {
      break;
    }
    webcam_->Watermark(&frame);
    video.write(frame);
  }
  video.release();
  webcam_->CloseCamera();
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
