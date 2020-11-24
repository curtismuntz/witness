#include "witness/server/vision/imshow_wrapper.h"

#include "glog/logging.h"
#include "opencv2/imgcodecs/imgcodecs.hpp"

namespace witness::server::vision {

DEFINE_string(output_photo_dir, "/tmp", "Output photo directory");
DEFINE_string(imshow_photo_extension, ".jpg", "Photo extension");

unsigned int ImageManager::number_lookup(const std::string &name) {
  if (_map.find(name) == _map.end()) {
    auto insertme = std::pair<std::string, unsigned int>(name, 0);
    _map.insert(insertme);
  } else {
    _map[name] = _map[name] + 1;
  }
  return _map[name];
}

void ImageManager::imshow(const std::string &name, const cv::Mat &img) {
  auto img_number = number_lookup(name);
  auto fname = FLAGS_output_photo_dir + "/" + name + "_" + std::to_string(img_number) +
               FLAGS_imshow_photo_extension;
  DLOG(INFO) << "Writing file: " << fname << std::endl;
  cv::imwrite(fname, img);
}

}  // namespace witness::server::vision
