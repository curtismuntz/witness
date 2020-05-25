#include <stdlib.h>

#include "witness/server/file_operations/file_operations.h"

namespace witness::server::file_operations {

void ClearDir(const std::string &fullpath) {
  for (auto &p : std::experimental::filesystem::directory_iterator(fullpath)) {
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

// If no extension is provided, it won't append the extension!
std::string CreatePathString(const std::string &directory, const std::string &filename,
                             const std::string extension /* = "" */) {
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
  auto hostvar = getenv("HOSTNAME");
  auto hostname = hostvar ? hostvar : "local";
  std::string fname = hostname + std::string{"_"};

  if (requested_fname != "") {
    fname = fname + requested_fname;
  } else {
    fname = fname + CurrentTimeString();
  }

  return CreatePathString(media_dir, fname, ext);
}

std::vector<std::string> ListDir(const std::string &media_path, const std::string &photo_ext,
                                 const std::string &video_ext) {
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

void MakeDir(const std::string &fullpath) {
  std::experimental::filesystem::create_directory(fullpath);
}

bool exists(const std::string fname) {
  return std::experimental::filesystem::exists(fname);
}

}  // namespace witness::server::file_operations
