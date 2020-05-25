#pragma once

#include <memory>
#include <string>
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness::server::webcam::actions {

class VideoRecorder : public witness::server::webcam::actions::WebcamAction {
 public:
  VideoRecorder(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname);
  bool Loop() override;

 private:
  const std::string fname_;
};

}  // namespace witness::server::webcam::actions
