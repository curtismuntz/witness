#pragma once

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

class Monitor : public witness::server::webcam::actions::WebcamAction {
 public:
  Monitor(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname);
  bool Loop() override;
 private:
  bool MotionDetected(cv::Mat *foreground_mask);
  const std::string fname_;
};

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
