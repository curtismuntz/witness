#pragma once

#include <memory>
#include <string>
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

class VideoRecorder : public witness::server::webcam::actions::WebcamAction {
 public:
  VideoRecorder(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname);
  bool Loop() override;

 private:
  const std::string fname_;
};

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
