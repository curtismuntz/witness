#pragma once

#include <memory>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc++/server.h"

#include "witness/api/calibration.grpc.pb.h"
#include "witness/server/vision/calibration.h"
#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

using grpc::ServerWriter;
using witness::api::StartCalibrationReply;

class Calibrate : public witness::server::webcam::actions::WebcamAction {
 public:
  Calibrate(std::shared_ptr<witness::webcam::Webcam> webcam,
            const witness::server::vision::CalibrateParameters &params);
  bool Loop() override;

 private:
  witness::server::vision::Calibrator calibrator_;
  unsigned int samples_;
  unsigned int minimum_calibration_samples_;
};

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
