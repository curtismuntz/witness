#pragma once

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <memory>
#include <string>
#include <thread>

#include "witness/api/witness.grpc.pb.h"
#include "witness/api/calibration.grpc.pb.h"
#include "witness/api/tracking.grpc.pb.h"
#include "witness/server/version.h"
#include "witness/server/webcam/actions/webcam_action.h"
#include "witness/server/webcam/webcam.h"

namespace witness {
namespace server {

using grpc::Server;
using grpc::ServerAsyncWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using witness::api::CameraRotationReply;
using witness::api::CameraRotationRequest;
using witness::api::ClearDataReply;
using witness::api::ClearDataRequest;
using witness::api::Error;
using witness::api::FileListReply;
using witness::api::GetFileListRequest;
using witness::api::OpenWebcamReply;
using witness::api::OpenWebcamRequest;
using witness::api::ServerState;
using witness::api::ServerStateReply;
using witness::api::ServerStateRequest;
using witness::api::StartAprilTrackingReply;
using witness::api::StartAprilTrackingRequest;
using witness::api::StartCalibrationRequest;
using witness::api::StartCalibrationReply;
using witness::api::StartMonitorReply;
using witness::api::StartMonitorRequest;
using witness::api::StartRecordingReply;
using witness::api::StartRecordingRequest;
using witness::api::StartTimelapseReply;
using witness::api::StartTimelapseRequest;
using witness::api::StopRecordingReply;
using witness::api::StopRecordingRequest;
using witness::api::TakePhotoReply;
using witness::api::TakePhotoRequest;
using witness::api::VersionReply;
using witness::api::VersionRequest;
using witness::api::WitnessCameraService;

class WitnessService final : public WitnessCameraService::Service {
 public:
  WitnessService(std::string media_dir, const std::string &watermark_text);

 private:
  Status StartTimelapse(ServerContext *context, const StartTimelapseRequest *request,
                        StartTimelapseReply *reply) override;
  Status StartRecording(ServerContext *context, const StartRecordingRequest *request,
                        StartRecordingReply *reply) override;
  Status StopRecording(ServerContext *context, const StopRecordingRequest *request,
                       StopRecordingReply *reply) override;
  Status TakePhoto(ServerContext *context, const TakePhotoRequest *request,
                   TakePhotoReply *reply) override;
  Status OpenWebcam(ServerContext *context, const OpenWebcamRequest *request,
                    OpenWebcamReply *reply) override;
  Status GetFileList(ServerContext *context, const GetFileListRequest *request,
                     ServerWriter<FileListReply> *writer) override;
  Status GetServerState(ServerContext *context, const ServerStateRequest *request,
                        ServerStateReply *reply) override;
  Status ClearData(ServerContext *context, const ClearDataRequest *request,
                   ClearDataReply *writer) override;
  Status StartMonitor(ServerContext *context, const StartMonitorRequest *request,
                      StartMonitorReply *writer) override;
  Status GetServerVersion(ServerContext *context, const VersionRequest *request,
                          VersionReply *writer) override;
  Status SetCameraRotation(ServerContext *context, const CameraRotationRequest *request,
                           CameraRotationReply *writer) override;
  Status StartAprilTracking(ServerContext *context, const StartAprilTrackingRequest *request,
                            ServerWriter<StartAprilTrackingReply> *writer) override;
  Status StartCalibration(ServerContext *context, const StartCalibrationRequest *request,
                            ServerWriter<StartCalibrationReply> *writer) override;
  std::shared_ptr<witness::webcam::Webcam> webcam_;
  std::unique_ptr<witness::server::webcam::actions::WebcamAction> webcam_action_;
  std::string media_dir_;
};

void RunServer(const std::string &media_dir);

}  // namespace server
}  // namespace witness
