#pragma once
#include <glog/logging.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"
#include <memory>

namespace witness {
namespace webcam {

// Manager and actions for common camera acquisition and adjustments
class Webcam {
public:
  Webcam(int camera_rotation_degrees)
  : rotation_degrees_(camera_rotation_degrees)
  {};

  ~Webcam() {CloseCamera();};

  bool OpenCamera(int camera_id = 1);
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

private:
  bool active_;
  int rotation_degrees_;
};

} // webcam
} // witness
