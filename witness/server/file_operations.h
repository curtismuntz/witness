#pragma once

#include <experimental/filesystem>
#include <glog/logging.h>

#include <chrono>
#include <iomanip>
#include <vector>
#include <string>

namespace witness {
namespace file_operations {

static bool ValidateExtension(const char *flagname, const std::string &value) {
  std::experimental::filesystem::path path(value);
  if (!path.has_extension()) {
    LOG(ERROR) << "Extension needs a \".\" prefix.";
    return false;
  }
  return true;
}

void ClearDir(std::string media_dir);
std::vector<std::string> ListDir(std::string media_path, std::string photo_ext,
                                std::string video_ext);
std::string CurrentTimeString();
std::string CreatePath(const std::string &directory, const std::string &filename,
                       const std::string extension);
std::string DecideFilename(const std::string &media_dir, const std::string &requested_fname,
                                  const std::string &ext);
}  // namespace file_operations
}  // namespace witness
