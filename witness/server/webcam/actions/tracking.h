#pragma once

#include <memory>
#include <string>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc++/server.h"

#include "witness/api/witness.grpc.pb.h"
#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {
namespace webcam {
namespace actions {

using grpc::ServerWriter;
using witness::api::StartAprilTrackingReply;

struct TrackingParameters {
  std::string fname_;
  std::string tag_id_;
  bool record_;
  TrackingParameters() : fname_(""), tag_id_("") {}
};

class Tracking : public witness::server::webcam::actions::WebcamAction {
 public:
  Tracking(std::shared_ptr<witness::webcam::Webcam> webcam, const TrackingParameters &params,
           ServerWriter<StartAprilTrackingReply> *writer);
  bool Loop() override;

 private:
  const TrackingParameters params_;
  ServerWriter<StartAprilTrackingReply> *writer_;
};

}  // namespace actions
}  // namespace webcam
}  // namespace server
}  // namespace witness
