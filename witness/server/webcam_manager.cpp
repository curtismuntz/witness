#include <glog/logging.h>
#include <chrono>
#include <iostream>
#include <vector>

#include "witness/server/file_operations.h"
#include "witness/server/webcam_manager.h"

namespace witness {
namespace server {
namespace webcam_manager {

WebcamManager::WebcamManager(int camera_rotation_degrees)
    : camera_(nullptr),
      worker_thread_(nullptr),
      camera_recording_(false),
      monitoring_(false),
      rotation_degrees_(camera_rotation_degrees) {}
WebcamManager::~WebcamManager() { CloseCamera(); }

bool WebcamManager::OpenCamera(int camera_id /* = 0 */) {
  if (camera_) {
    LOG(INFO) << "camera already opened" << std::endl;
    return false;
  }
  LOG(INFO) << "opening camera " << camera_id << std::endl;
  camera_ = std::make_unique<cv::VideoCapture>(camera_id);

  if (!camera_->isOpened()) {
    LOG(ERROR) << "unable to open camera" << std::endl;
    return false;
  }
  return true;
}

void WebcamManager::CloseCamera() {
  if (camera_) {
    camera_->release();
  }
  camera_.reset(nullptr);
}

bool WebcamManager::SetParameter(const int cv_parameter, const int value) {
  if (camera_) {
    return camera_->set(cv_parameter, value);
  }
  return false;
}

bool WebcamManager::SaveImage(const std::string &desired_filename) {
  OpenCamera();
  cv::Mat frame;
  ReadFrame(&frame);
  CloseCamera();
  Watermark(&frame);

  bool success = !frame.empty();
  if (success) {
    success = success && cv::imwrite(desired_filename, frame);
  }

  return success;
}

void WebcamManager::ReadGrey(cv::Mat *frame, cv::Mat *grey) {
  if (ReadFrame(frame)) {
    cv::cvtColor(*frame, *grey, CV_BGR2GRAY);
  }
}

bool WebcamManager::ReadFrame(cv::Mat *frame) {
  if (camera_->isOpened()) {
    camera_->read(*frame);

    if (frame->empty()) {
      LOG(INFO) << "empty frame" << std::endl;
      return false;
    }

    if (rotation_degrees_ != 0) {
      static const double scale = 1.0;
      auto center = cv::Point2f(frame->cols / 2., frame->rows / 2.);
      auto M = cv::getRotationMatrix2D(center, rotation_degrees_, scale);
      cv::warpAffine(*frame, *frame, M, frame->size());
    }
    return true;
  } else {
    return false;
  }
}

void WebcamManager::TimelapseLoop(const std::string &file_path, int sleep_for) {
  OpenCamera();

  cv::Mat frame;
  auto frame_number = 0u;

  auto width = camera_->get(cv::CAP_PROP_FRAME_WIDTH);
  auto height = camera_->get(cv::CAP_PROP_FRAME_HEIGHT);

  cv::VideoWriter video(file_path, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(width, height));
  while (camera_recording_ && camera_->isOpened()) {
    // get a new frame from camera
    if (!ReadFrame(&frame)) {
      break;
    }
    std::chrono::seconds sec(sleep_for);
    LOG(INFO) << "TIMELAPSE running: Frame " << frame_number << " Sleep: " << sec.count() << "s";
    std::this_thread::sleep_for(sec);
    Watermark(&frame);
    video.write(frame);
    frame_number++;
  }
  video.release();
  CloseCamera();
}

cv::VideoWriter WebcamManager::CreateVideoObject(const std::string &fname) {
  auto fps = camera_->get(cv::CAP_PROP_FPS);
  auto width = camera_->get(cv::CAP_PROP_FRAME_WIDTH);
  auto height = camera_->get(cv::CAP_PROP_FRAME_HEIGHT);

#ifdef RPI_CAM
  auto codec = CV_FOURCC('M', 'J', 'P', 'G');
  cv::VideoWriter video(fname, codec, fps, cv::Size(width, height));
#else
  auto codec = camera_->get(cv::CAP_PROP_FOURCC);
  cv::VideoWriter video(fname, codec, fps, cv::Size(width, height));
#endif

  LOG(INFO) << "FPS: " << fps << " Width: " << width << " Height: " << height
            << " Codec: " << static_cast<int>(codec);
  return video;
}

void WebcamManager::VideoLoop(const std::string &fname) {
  OpenCamera();

  auto video = CreateVideoObject(fname);

  cv::Mat frame;
  while (camera_recording_ && camera_->isOpened()) {
    // get a new frame from camera
    if (ReadFrame(&frame)) {
      break;
    }
    Watermark(&frame);
    video.write(frame);
  }
  video.release();
  CloseCamera();
}

bool WebcamManager::StartTimelapse(const std::string &desired_filename, int sleep_for) {
  if (IsRecording() || IsMonitoring()) {
    return false;
  }
  camera_recording_ = true;
  // start video thread;
  worker_thread_ = std::make_unique<std::thread>(&WebcamManager::TimelapseLoop, this,
                                                 desired_filename, sleep_for);
  return true;
}

bool WebcamManager::StartRecording(const std::string &desired_filename) {
  if (IsRecording() || IsMonitoring()) {
    return false;
  }
  camera_recording_ = true;
  // start video thread;
  worker_thread_ = std::make_unique<std::thread>(&WebcamManager::VideoLoop, this, desired_filename);
  return true;
}

bool WebcamManager::StopRecording() {
  if (IsRecording()) {
    camera_recording_ = false;
  }

  if (IsMonitoring()) {
    monitoring_ = false;
  }

  if (worker_thread_) {
    worker_thread_->join();
    worker_thread_.reset(nullptr);
  }

  CloseCamera();
  return true;
}

void WebcamManager::Watermark(cv::Mat *img) {
  cv::putText(*img, "https://murt.is", cv::Point(30, 30), cv::FONT_HERSHEY_COMPLEX, 0.8,
              cv::Scalar(0, 165, 255), 1, CV_AA);
}

void WebcamManager::WatermarkTime(cv::Mat *img) {
  Watermark(img);
  cv::putText(*img, witness::server::file_operations::CurrentTimeString(), cv::Point(30, 300),
              cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 165, 255), 1, CV_AA);
}

bool WebcamManager::StartMonitoring(const std::string &desired_filename) {
  if (IsRecording() || IsMonitoring()) {
    return false;
  }

  monitoring_ = true;
  worker_thread_ =
      std::make_unique<std::thread>(&WebcamManager::MonitorLoop, this, desired_filename);
  return true;
}

bool WebcamManager::MotionDetected(cv::Mat *foreground_mask) {
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

void WebcamManager::MonitorLoop(const std::string &desired_filename) {
  OpenCamera();

  cv::Mat foreground_mask;
  cv::Mat grey;
  cv::Mat frame;
  int frame_number = 0;

  cv::Ptr<cv::BackgroundSubtractor> pBackSub;
  pBackSub = cv::createBackgroundSubtractorMOG2();

  // seed background subtractor
  ReadGrey(&frame, &grey);
  pBackSub->apply(grey, foreground_mask);

  auto video = CreateVideoObject(desired_filename);

  while (monitoring_ && camera_->isOpened()) {
    ReadGrey(&frame, &grey);
    pBackSub->apply(grey, foreground_mask);

    if (MotionDetected(&foreground_mask)) {
      LOG(INFO) << "MOTION DETECTED at frame " << frame_number;
      WatermarkTime(&frame);
      video.write(frame);
    }
    ++frame_number;
  }

  video.release();
  CloseCamera();
}

}  // namespace webcam_manager
}  // namespace server
}  // namespace witness
