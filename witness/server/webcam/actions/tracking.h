#pragma once

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

class Tracking : public witness::server::webcam::actions::WebcamAction {
 public:
  Tracking(std::shared_ptr<witness::webcam::Webcam> webcam, const std::string &fname,
            const std::string &tag_id);
  bool Loop() override;
 private:
  const std::string fname_;
  const std::string tag_id_;
};

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
