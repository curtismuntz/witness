#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"

namespace witness {
namespace server {
namespace webcam_manager {

class WebcamManager {
 public:
  explicit WebcamManager(int camera_rotation_degrees);
  ~WebcamManager();

  bool OpenCamera(int camera_id = 0);
  void CloseCamera();
  bool SetParameter(const int cv_parameter, const int value);
  bool SaveImage(const std::string &desired_filename);
  bool UnmanagedSaveImage(const std::string &desired_filename);
  bool StartRecording(const std::string &desired_filename);
  bool StartTimelapse(const std::string &folder_path, const std::string &video_ext,
                      const std::string &photo_ext, int sleep_for);
  bool StartMonitoring(const std::string &desired_filename);
  bool StopRecording();
  bool IsRecording() { return camera_recording_; }
  bool IsMonitoring() { return monitoring_; }
  bool IsTracking() {return tracking_; }
  bool IsActive() { return IsRecording() || IsMonitoring() || IsTracking(); }
  void SetCameraRotation(const int degrees) { rotation_degrees_ = degrees; }
  bool StartAprilTracking(const std::string &tag_id);
 private:
  void VideoLoop(const std::string &fname);
  void TrackingLoop(const std::string &tag_id);
  void TimelapseLoop(const std::string &folder_path, const std::string &video_ext,
                     const std::string &photo_ext, int sleep_for);
  void MonitorLoop(const std::string &fname);
  bool MotionDetected(cv::Mat *foreground_mask);
  cv::VideoWriter CreateVideoObject(const std::string &fname);
  bool ReadFrame(cv::Mat *frame);
  void ReadGrey(cv::Mat *frame, cv::Mat *grey);
  void Watermark(cv::Mat *img);
  void WatermarkTime(cv::Mat *img);

  std::unique_ptr<cv::VideoCapture> camera_;
  std::unique_ptr<std::thread> worker_thread_;
  bool camera_recording_;
  bool monitoring_;
  bool tracking_;
  int rotation_degrees_;
};

}  // namespace webcam_manager
}  // namespace server
}  // namespace witness
