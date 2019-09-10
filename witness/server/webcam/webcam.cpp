#include <glog/logging.h>
#include "witness/server/webcam/webcam.h"
#include "witness/server/common/file_operations.h"

namespace witness {
namespace webcam {


bool Webcam::SaveImage(const std::string &desired_filename) {
  OpenCamera();
  auto success = UnmanagedSaveImage(desired_filename);
  CloseCamera();
  return success;
}

bool Webcam::UnmanagedSaveImage(const std::string &desired_filename) {
  cv::Mat frame;
  ReadFrame(&frame);
  Watermark(&frame);

  bool success = !frame.empty();
  if (success) {
    success = success && cv::imwrite(desired_filename, frame);
  }

  return success;
}

void Webcam::CloseCamera() {
  if (camera_) {
    camera_->release();
  }
  camera_.reset(nullptr);
}

bool Webcam::SetParameter(const int cv_parameter, const int value) {
  if (camera_) {
    return camera_->set(cv_parameter, value);
  }
  return false;
}

bool Webcam::ReadFrame(cv::Mat *frame) {
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

void Webcam::ReadGrey(cv::Mat *frame, cv::Mat *grey) {
  if (ReadFrame(frame)) {
    cv::cvtColor(*frame, *grey, CV_BGR2GRAY);
  }
}

cv::VideoWriter Webcam::CreateVideoObject(const std::string &fname) {
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


void Webcam::Watermark(cv::Mat *img) {
  cv::putText(*img, "https://murt.is", cv::Point(30, 30), cv::FONT_HERSHEY_COMPLEX, 0.8,
              cv::Scalar(0, 165, 255), 1, CV_AA);
}

void Webcam::WatermarkTime(cv::Mat *img) {
  Watermark(img);
  cv::putText(*img, witness::server::file_operations::CurrentTimeString(), cv::Point(30, 300),
              cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 165, 255), 1, CV_AA);
}

bool Webcam::OpenCamera(int camera_id /* = 0 */) {
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

} // webcam
} // witness
