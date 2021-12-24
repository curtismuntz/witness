#include <stdlib.h>

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "witness/server/file_operations/file_operations.h"

namespace witness::server::file_operations {

void ClearDir(const std::string &fullpath) {
  for (auto &p : std::experimental::filesystem::directory_iterator(fullpath)) {
    LOG(INFO) << "Removing " << p.path();
    std::experimental::filesystem::remove_all(p.path());
  }
}

std::string CurrentTimeString() {
  auto now = absl::Now();
  auto tz = absl::LocalTimeZone();
  return absl::FormatTime("%Y-%m-%d_%H-%M-%S", now, tz);
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

  if (!requested_fname.empty()) {
    fname = fname + requested_fname;
  } else {
    fname = fname + CurrentTimeString();
  }

  return CreatePathString(media_dir, fname, ext);
}

std::vector<std::string> ListDir(const std::string &media_path, std::unordered_set<std::string> extensions){
  std::vector<std::string> file_list;
  for (const auto &p : std::experimental::filesystem::directory_iterator(media_path)) {
    auto fext = p.path().extension().string();
    if (extensions.find(fext) != extensions.end()) {
      auto path_string = p.path().string();
      DLOG(INFO) << "Found file at " << path_string;
      file_list.push_back(path_string);
    }
  }
  return file_list;
}

std::string MakeTimeDir(const std::string& base_directory) {
  std::experimental::filesystem::path new_folder(std::experimental::filesystem::path{base_directory} / std::experimental::filesystem::path{CurrentTimeString()});
  MakeDir(new_folder);
  return new_folder;
}

void MakeDir(const std::string &fullpath) {
  LOG(INFO)<< "Creating folder " << fullpath << std::endl;
  std::experimental::filesystem::create_directory(fullpath);
}

bool exists(const std::string& fname) {
  return std::experimental::filesystem::exists(fname);
}

}  // namespace witness::server::file_operations
