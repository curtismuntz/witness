#pragma once
#include <memory>
#include <string>
#include "glog/logging.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"

namespace witness::webcam {

// Manager and actions for common camera acquisition and adjustments
class Webcam {
 public:
  explicit Webcam(int camera_rotation_degrees)
      : calibrated_(false), rotation_degrees_(camera_rotation_degrees) {
    // Disable autofocus.
    SetParameter(cv::CAP_PROP_AUTOFOCUS, 0);
  }

  ~Webcam() { CloseCamera(); }

  bool OpenCamera(int camera_id = 0);
  bool IsCalibrated() { return calibrated_; }
  void SetCalibration(const cv::Mat &m) { camera_intrisics_ = m; }
  cv::Mat GetCalibration() { return camera_intrisics_; }
  void CloseCamera();
  bool SetParameter(const int cv_parameter, const int value);
  bool SaveImage(const std::string &desired_filename);
  bool UnmanagedSaveImage(const std::string &desired_filename);
  cv::VideoWriter CreateVideoObject(const std::string &fname);
  bool ReadFrame(cv::Mat *frame);
  void ReadGrey(cv::Mat *frame, cv::Mat *grey);
  void Watermark(cv::Mat *img);
  void WatermarkTime(cv::Mat *img);
  void SetCameraRotation(const int degrees) { rotation_degrees_ = degrees; }
  std::unique_ptr<cv::VideoCapture> camera_;
  void SetWatermarkString(const std::string &newstring) { watermark_string_ = newstring; }

 private:
  bool active_;
  bool calibrated_;
  int rotation_degrees_;
  cv::Mat camera_intrisics_;
  std::string watermark_string_ = "https://murt.is";
};

}  // namespace witness::webcam
