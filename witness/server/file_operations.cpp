#include <stdlib.h>

#include "witness/server/file_operations.h"

namespace witness {
namespace file_operations {

void ClearDir(std::string media_dir) {
  for (auto &p : std::experimental::filesystem::directory_iterator(media_dir)) {
    LOG(INFO) << "Removing " << p.path();
    std::experimental::filesystem::remove_all(p.path());
  }
}

std::string CurrentTimeString() {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
  return oss.str();
}

std::string CreatePath(const std::string &directory, const std::string &filename,
                       const std::string extension) {
  std::experimental::filesystem::path path;
  std::experimental::filesystem::path dir(directory);
  std::experimental::filesystem::path fname(filename);
  std::experimental::filesystem::path ext(extension);

  path = dir / fname;
  path += ext;
  return path.string();
}

std::string DecideFilename(const std::string &media_dir, const std::string &requested_fname,
                                  const std::string &ext) {
  std::string fname = getenv("HOSTNAME") + std::string{"_"};

  if (requested_fname != "") {
    fname = fname + requested_fname;
  } else {
    fname = fname + CurrentTimeString();
  }

  return CreatePath(media_dir, fname, ext);
}

std::vector<std::string> ListDir(std::string media_path, std::string photo_ext,
                                std::string video_ext) {
  std::vector<std::string> file_list;
  for (const auto &p : std::experimental::filesystem::directory_iterator(media_path)) {
    auto fext = p.path().extension();
    if (fext == photo_ext || fext == video_ext) {
      auto path_string = p.path().string();
      LOG(INFO) << "Found file at " << path_string;
      file_list.push_back(path_string);
    }
  }
  return file_list;
}
}  // namespace file_operations
}  // namespace witness
