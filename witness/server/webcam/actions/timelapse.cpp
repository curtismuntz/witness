#include "witness/server/webcam/actions/timelapse.h"

#include <algorithm>

#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "witness/server/vision/create_timelapse.h"

namespace witness::server::webcam::actions {

TimeLapse::TimeLapse(std::shared_ptr<witness::webcam::Webcam> webcam,
                     const std::string &folder_path, const std::string &video_ext,
                     const std::string &photo_ext, int sleep_for)
    : WebcamAction(webcam),
      folder_(folder_path),
      video_ext_(video_ext),
      photo_ext_(photo_ext),
      sleep_for_(sleep_for) {}

bool TimeLapse::Loop() {
  webcam_->OpenCamera();
  cv::Mat frame;
  auto frame_number = 0u;

  // Save photos to the path and sleep between.
  while (working_ && webcam_->camera_->isOpened()) {
    auto img_name = std::string{folder_ + "/" + std::to_string(frame_number) + photo_ext_};
    std::chrono::seconds sec(sleep_for_);

    LOG(INFO) << "Saving timpelapse photo " << img_name << "; Sleep: " << sec.count() << "s";
    webcam_->UnmanagedSaveImage(img_name);

    std::this_thread::sleep_for(sec);
    frame_number++;
  }

  auto width = webcam_->camera_->get(cv::CAP_PROP_FRAME_WIDTH);
  auto height = webcam_->camera_->get(cv::CAP_PROP_FRAME_HEIGHT);
  webcam_->CloseCamera();

  auto video_name = std::string{folder_ + video_ext_};
  vision::create_timelapse(folder_, photo_ext_, video_name);
}

}  // namespace witness::server::webcam::actions
