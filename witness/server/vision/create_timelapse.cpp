
#include <experimental/filesystem>

#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

#include "glog/logging.h"
#include "witness/server/file_operations/file_operations.h"
#include "third_party/alphanum/alphanum.hpp"

#include "witness/server/vision/create_timelapse.h"

namespace witness::server::vision {

bool create_timelapse(const std::string& folder, const std::string& photo_extension) {
  std::experimental::filesystem::path video_name;
  const std::string video_ext{".avi"};
  const std::string filename{"a_timelapse"};
  video_name = std::experimental::filesystem::path{folder} / (filename + video_ext);
  return create_timelapse(folder, photo_extension, video_name.string());
}

bool create_timelapse(const std::string& folder, const std::string& photo_extension, const std::string& video_name) {

  if (!std::experimental::filesystem::exists(folder) || !std::experimental::filesystem::is_directory(folder)) {
    LOG(ERROR) << "Requested folder for timelapse image does not exist: " << folder;
    return false;
  }

  // Iterate over folder path and add frames to a video object.
  auto file_vector = witness::server::file_operations::ListDir(folder, std::unordered_set<std::string>{photo_extension});

  if(file_vector.empty()) {
    LOG(ERROR) << "No files of extension " << photo_extension << " found in " << folder;
    return false;
  }

  std::sort(file_vector.begin(), file_vector.end(), doj::alphanum_less<std::string>());

  // Read first image to get the height/width of images. It is assumed all files have same shape.
  auto first_image = cv::imread(file_vector[0]);
  int width = first_image.cols;
  int height = first_image.rows;
  int framerate = 30;

  LOG(INFO) << "Writing timelapse video: " << video_name;
  cv::VideoWriter video(video_name, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), framerate,
                        cv::Size(width, height));
  cv::Mat mat;
  for (const auto& i : file_vector) {
    DLOG(INFO) << "Processing file " << i;
    mat = cv::imread(i);
    if(mat.rows != height || mat.cols != width) {
      LOG(ERROR) << "Error processing file " << i;
      continue;
    }
    video.write(mat);
  }
  video.release();
  return true;
}

}
