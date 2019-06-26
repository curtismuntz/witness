#include <glog/logging.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

#include "third_party/alphanum/alphanum.hpp"
#include "witness/server/file_operations.h"
#include "witness/server/webcam_manager.h"

extern "C" {
#include "apriltag.h"
#include "tagStandard41h12.h"
}

namespace witness {
namespace server {
namespace webcam_manager {

WebcamManager::WebcamManager(int camera_rotation_degrees)
    : camera_(nullptr),
      worker_thread_(nullptr),
      camera_recording_(false),
      monitoring_(false),
      tracking_(false),
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
  auto success = UnmanagedSaveImage(desired_filename);
  CloseCamera();
  return success;
}

bool WebcamManager::UnmanagedSaveImage(const std::string &desired_filename) {
  cv::Mat frame;
  ReadFrame(&frame);
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

void WebcamManager::TimelapseLoop(const std::string &folder_path, const std::string &video_ext,
                                  const std::string &photo_ext, int sleep_for) {
  OpenCamera();
  cv::Mat frame;
  auto frame_number = 0u;

  // Save photos to the path and sleep between.
  while (camera_recording_ && camera_->isOpened()) {
    auto img_name = std::string{folder_path + "/" + std::to_string(frame_number) + photo_ext};
    std::chrono::seconds sec(sleep_for);

    LOG(INFO) << "Saving timpelapse photo " << img_name << "; Sleep: " << sec.count() << "s";
    UnmanagedSaveImage(img_name);

    std::this_thread::sleep_for(sec);
    frame_number++;
  }

  auto width = camera_->get(cv::CAP_PROP_FRAME_WIDTH);
  auto height = camera_->get(cv::CAP_PROP_FRAME_HEIGHT);
  CloseCamera();

  auto video_name = std::string{folder_path + video_ext};
  LOG(INFO) << "Writing timelapse video: " << video_name;
  cv::VideoWriter video(video_name, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(width, height));

  // Iterate over folder path and add frames to a video object.
  auto file_vector = file_operations::ListDir(folder_path, photo_ext, video_ext);
  std::sort(file_vector.begin(), file_vector.end(), doj::alphanum_less<std::string>());
  for (auto i : file_vector) {
    auto mat = cv::imread(i);
    video.write(mat);
  }
  video.release();
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
    if (!ReadFrame(&frame)) {
      break;
    }
    Watermark(&frame);
    video.write(frame);
  }
  video.release();
  CloseCamera();
}

bool WebcamManager::StartTimelapse(const std::string &folder_path, const std::string &video_ext,
                                   const std::string &photo_ext, int sleep_for) {
  if (IsActive()) {
    return false;
  }
  camera_recording_ = true;
  // start video thread;
  worker_thread_ = std::make_unique<std::thread>(&WebcamManager::TimelapseLoop, this, folder_path,
                                                 video_ext, photo_ext, sleep_for);
  return true;
}

bool WebcamManager::StartRecording(const std::string &desired_filename) {
  if (IsActive()) {
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

  if (IsTracking()) {
    tracking_ = false;
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

void WebcamManager::TrackingLoop(const std::string &tag_id) {
  OpenCamera();
  auto desired_filename = "tracking.avi";
  cv::Mat gray;
  cv::Mat frame;
  int frame_number = 0;


  auto video = CreateVideoObject(desired_filename);
  apriltag_family_t *tf = NULL;
  tf = tagStandard41h12_create();
  apriltag_detector_t *td = apriltag_detector_create();
  apriltag_detector_add_family(td, tf);

  // TODO: configurables!
  td->quad_decimate = 2.0;
  td->quad_sigma = 0.0;
  td->nthreads = 1;
  td->debug = false;
  td->refine_edges = true;

  while (tracking_ && camera_->isOpened()) {
    ReadGrey(&frame, &gray);
    // Make an image_u8_t header for the Mat data
    image_u8_t im = { .width = gray.cols,
        .height = gray.rows,
        .stride = gray.cols,
        .buf = gray.data
    };

    zarray_t *detections = apriltag_detector_detect(td, &im);


    LOG(INFO) << zarray_size(detections) << "tags detected";

    // Draw detection outlines
    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);
        cv::line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Scalar(0, 0xff, 0), 2);
        cv::line(frame, cv::Point(det->p[0][0], det->p[0][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0, 0, 0xff), 2);
        cv::line(frame, cv::Point(det->p[1][0], det->p[1][1]),
                 cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Scalar(0xff, 0, 0), 2);
        cv::line(frame, cv::Point(det->p[2][0], det->p[2][1]),
                 cv::Point(det->p[3][0], det->p[3][1]),
                 cv::Scalar(0xff, 0, 0), 2);

        std::stringstream ss;
        ss << det->id;
        cv::String text = ss.str();
        int fontface = CV_FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontscale = 1.0;
        int baseline;
        cv::Size textsize = cv::getTextSize(text, fontface, fontscale, 2,
                                        &baseline);
        cv::putText(frame, text, cv::Point(det->c[0]-textsize.width/2,
                                   det->c[1]+textsize.height/2),
                fontface, fontscale, cv::Scalar(0xff, 0x99, 0), 2);
    }
    apriltag_detections_destroy(detections);
    video.write(frame);
    ++frame_number;
  }
  apriltag_detector_destroy(td);
  tagStandard41h12_destroy(tf);
  video.release();


  CloseCamera();
}

bool WebcamManager::StartAprilTracking(const std::string &tag_id) {
  if (IsActive()) {
    return false;
  }
  tracking_ = true;
  worker_thread_ =
      std::make_unique<std::thread>(&WebcamManager::TrackingLoop, this, tag_id);
  return true;
}

}  // namespace webcam_manager
}  // namespace server
}  // namespace witness
