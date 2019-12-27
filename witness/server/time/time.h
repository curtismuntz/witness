#pragma once

#include <chrono>
#include <utility>

namespace witness {
namespace server {
namespace time {

std::pair<std::chrono::seconds, std::chrono::nanoseconds> current_time();

}  // namespace time
}  // namespace server
}  // namespace witness
