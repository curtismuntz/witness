#pragma once

#include "string"

namespace witness::server::vision {
bool create_timelapse(const std::string& folder, const std::string& photo_extension);
bool create_timelapse(const std::string& folder, const std::string& photo_extension, const std::string& video_name);
}
