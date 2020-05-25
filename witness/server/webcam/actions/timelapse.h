#pragma once

#include <memory>
#include <string>
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness::server::webcam::actions {

class TimeLapse : public witness::server::webcam::actions::WebcamAction {
 public:
  TimeLapse(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &folder_path,
            const std::string &video_ext, const std::string &photo_ext, int sleep_for);
  bool Loop() override;

 private:
  const std::string folder_;
  const std::string video_ext_;
  const std::string photo_ext_;
  const int sleep_for_;
};

}  // namespace witness::server::webcam::actions
