#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"

class WebcamManager {
 public:
  WebcamManager();
  ~WebcamManager();

  bool OpenCamera(int camera_id = 0);
  void CloseCamera();
  bool SetParameter(const int cv_parameter, const int value);
  bool SaveImage(const std::string &desired_filename);
  bool StartRecording(const std::string &desired_filename);
  bool StartTimelapse(const std::string &desired_filename, int sleep_for);
  bool StartMonitoring(const std::string &desired_filename);
  bool StopRecording();
  bool IsRecording() { return camera_recording_; }
  bool IsMonitoring() { return monitoring_; }

 private:
  void VideoLoop(const std::string &fname);
  void TimelapseLoop(const std::string &fname, int sleep_for);
  void MonitorLoop(const std::string &fname);
  bool MotionDetected(cv::Mat *foreground_mask);
  cv::VideoWriter CreateVideoObject(const std::string &fname);

  void ReadFrame(cv::Mat *frame, cv::Mat *grey);
  cv::Mat GrabFrame();
  void Watermark(cv::Mat *img);
  void WatermarkTime(cv::Mat *img);

  std::unique_ptr<cv::VideoCapture> camera_;
  std::unique_ptr<std::thread> worker_thread_;
  bool camera_recording_;
  bool monitoring_;
};
