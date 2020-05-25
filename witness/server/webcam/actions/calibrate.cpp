#include "witness/server/webcam/actions/calibrate.h"
#include <vector>
#include "witness/server/file_operations/file_operations.h"

namespace witness::server::webcam::actions {

Calibrate::Calibrate(std::shared_ptr<witness::webcam::Webcam> webcam,
                     const witness::server::vision::CalibrateParameters &params)
    : WebcamAction(webcam),
      calibrator_(params),
      samples_(0),
      minimum_calibration_samples_(params.minimum_calibration_samples) {}

bool Calibrate::Loop() {
  webcam_->OpenCamera();
  cv::Mat frame;
  LOG(INFO) << "Starting calibration loop";
  while ((working_ && webcam_->camera_->isOpened()) && samples_ < minimum_calibration_samples_) {
    // get a new frame from camera
    if (!webcam_->ReadFrame(&frame)) {
      LOG(INFO) << "Camera read error";
      break;
    }

    // Do a quick check to see if we found a chessboard.
    std::vector<cv::Point2f> pointBuf;
    auto found = calibrator_.find_chess_board(frame, pointBuf);

    if (found) {
      ++samples_;
      LOG(INFO) << "Chessboard found. Adding frame to calibration data. Total samples: "
                << samples_;
      calibrator_.add_image(frame);
    }
  }

  if (samples_ >= minimum_calibration_samples_) {
    LOG(INFO) << minimum_calibration_samples_ << " samples gathered. Running calibration.";
    auto success = calibrator_.calibrate();
    if (success) {
      webcam_->SetCalibration(calibrator_.get_camera_matrix());
      return true;
    } else {
      LOG(ERROR) << "Calibration failed.";
    }
  } else {
    LOG(ERROR) << "Not enough samples gathered";
    return false;
  }
  Stop();
  return true;
}

}  // namespace witness::server::webcam::actions
