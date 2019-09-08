#include "witness/server/webcam/actions/timelapse.h"
#include "third_party/alphanum/alphanum.hpp"
#include "witness/server/common/file_operations.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

TimeLapse::TimeLapse(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &folder_path, const std::string &video_ext,
          const std::string &photo_ext, int sleep_for)
  : WebcamAction(webcam)
  , folder_(folder_path)
  , video_ext_(video_ext)
  , photo_ext_(photo_ext)
  , sleep_for_(sleep_for)
{};

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
  LOG(INFO) << "Writing timelapse video: " << video_name;
  cv::VideoWriter video(video_name, CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(width, height));

  // Iterate over folder path and add frames to a video object.
  auto file_vector = file_operations::ListDir(folder_, photo_ext_, video_ext_);
  std::sort(file_vector.begin(), file_vector.end(), doj::alphanum_less<std::string>());
  for (auto i : file_vector) {
    auto mat = cv::imread(i);
    video.write(mat);
  }
  video.release();
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
