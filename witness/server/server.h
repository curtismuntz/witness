#pragma once

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <thread>

#include "witness/api/witness.grpc.pb.h"
#include "witness/server/version.h"
#include "witness/server/webcam_manager.h"

namespace witness {
namespace server {

using grpc::Server;
using grpc::ServerWriter;
using grpc::ServerAsyncWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using witness::api::witness::WitnessCameraService;
using witness::api::witness::OpenWebcamRequest;
using witness::api::witness::OpenWebcamReply;
using witness::api::witness::ClearDataRequest;
using witness::api::witness::ClearDataReply;
using witness::api::witness::StartRecordingRequest;
using witness::api::witness::StartRecordingReply;
using witness::api::witness::StopRecordingRequest;
using witness::api::witness::StopRecordingReply;
using witness::api::witness::StartTimelapseRequest;
using witness::api::witness::StartTimelapseReply;
using witness::api::witness::GetFileListRequest;
using witness::api::witness::FileListReply;
using witness::api::witness::TakePhotoRequest;
using witness::api::witness::TakePhotoReply;
using witness::api::witness::ServerStateRequest;
using witness::api::witness::ServerStateReply;
using witness::api::witness::StartMonitorRequest;
using witness::api::witness::StartMonitorReply;
using witness::api::witness::Error;
using witness::api::witness::ServerState;
using witness::api::witness::VersionRequest;
using witness::api::witness::VersionReply;
using witness::api::witness::CameraRotationRequest;
using witness::api::witness::CameraRotationReply;

class WitnessService final : public WitnessCameraService::Service {
 public:
  WitnessService();

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
  witness::server::webcam_manager::WebcamManager webcam_;
};

void RunServer();

}  // namespace server
}  // namespace witness
