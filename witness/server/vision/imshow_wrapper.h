#pragma once

#include <string>
#include <unordered_map>
#include <utility>

#include "gflags/gflags.h"
#include "opencv2/core/core.hpp"

namespace witness::server::vision {

DECLARE_string(output_photo_dir);
DECLARE_string(imshow_photo_extension);

class ImageManager {
 public:
  ImageManager() {}
  void imshow(const std::string &name, const cv::Mat &img);
  unsigned int number_lookup(const std::string &name);
  void show(const std::string &name, const cv::Mat &img) { imshow(name, img); }

 private:
  // A mapping between image name and image number
  std::unordered_map<std::string, unsigned int> _map;
};

}  // namespace witness::server::vision
