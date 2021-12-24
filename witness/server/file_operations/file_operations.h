#pragma once

#include <glog/logging.h>
#include <experimental/filesystem>

#include <chrono>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_set>

namespace witness::server::file_operations {

static bool ValidateExtension(const char *flagname, const std::string &value) {
  std::experimental::filesystem::path path(value);
  if (!path.has_extension()) {
    LOG(ERROR) << "Extension needs a \".\" prefix.";
    return false;
  }
  return true;
}

std::string MakeTimeDir(const std::string& base_directory);
void MakeDir(const std::string &fullpath);
void ClearDir(const std::string &fullpath);

std::vector<std::string> ListDir(const std::string &media_path, std::unordered_set<std::string> extensions);
std::vector<std::string> ListDir(const std::string &media_path, const std::string &photo_ext,
                                 const std::string &video_ext);
std::string CurrentTimeString();
std::string CreatePathString(const std::string &directory, const std::string &filename,
                             const std::string extension = "");
std::string DecideFilename(const std::string &media_dir, const std::string &requested_fname,
                           const std::string &ext);
bool exists(const std::string& fname);

}  // namespace witness::server::file_operations
