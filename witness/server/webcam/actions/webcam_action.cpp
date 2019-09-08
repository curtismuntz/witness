#include <cstdint>
#include <memory>
#include <string>

#include "witness/server/webcam/actions/webcam_action.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {


bool WebcamAction::Start() {
  if(working_) {
    return false;
  }
  working_ = true;
  worker_ = std::make_unique<std::thread>(&WebcamAction::Loop, this);
  return true;
}

bool WebcamAction::Stop() {
  working_ = false;
  worker_->join();
  worker_.reset(nullptr);
  webcam_->CloseCamera();
  return true;
}

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
