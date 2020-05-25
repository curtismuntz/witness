#include "witness/server/time/time.h"

namespace witness::server::time {

std::pair<std::chrono::seconds, std::chrono::nanoseconds> current_time() {
  // https://stackoverflow.com/a/27678121/9711444
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::high_resolution_clock::now();
  auto duration = now.time_since_epoch();
  auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  duration -= seconds;
  auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
  return std::make_pair(seconds, nanos);
}

}  // namespace witness::server::time
