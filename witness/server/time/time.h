#pragma once

#include <chrono>
#include <utility>

namespace witness::server::time {

std::pair<std::chrono::seconds, std::chrono::nanoseconds> current_time();

}  // namespace witness::server::time
