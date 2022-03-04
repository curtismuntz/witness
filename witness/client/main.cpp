#include "witness/api/witness.pb.h"
#include "absl/flags/parse.h"
#include "absl/flags/flag.h"
#include "witness/drivers/amcrest.h"
#include <glog/logging.h>

#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>

#include "witness/api/witness.grpc.pb.h"

ABSL_FLAG(std::string, grpc_server_addr, "localhost:50051", "GRPC server address with port");
ABSL_FLAG(std::string, amcrest_connection_file, "/home/murt/amcrest_connections.textproto", "textproto for amcrest connection details");

using grpc::ClientContext;
using grpc::Status;
using grpc::Status;
using grpc::Channel;

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


class WitnessClient {
public:
  WitnessClient(std::shared_ptr<grpc::Channel> channel) : stub_(witness::api::WitnessCameraService::NewStub(channel)) {

  }
  int OpenCamera(const std::string& fname) {
    auto file_pb_optional = amcrest::LoadFromTextProto<AmcrestCameras>(fname);
    AmcrestConnection conn;
    if(!file_pb_optional.has_value()) {
      LOG(ERROR) << "Error reading from textproto file.";
      return -1;
    }

    for (auto connection: file_pb_optional.value().connections()) {
//      if(conn.camera_name())
      conn = connection;
    }

    OpenWebcamRequest request;
    request.set_allocated_amcrest_connection(&conn);
    OpenWebcamReply reply;
    ClientContext context;
    Status status = stub_->OpenWebcam(&context, request, &reply);
    // Act upon its status.
    if (status.ok()) {
      return 0;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

private:
  std::unique_ptr<witness::api::WitnessCameraService::Stub> stub_;
};

int main(int argc, char *argv[]) {
  absl::ParseCommandLine(argc, argv);
  WitnessClient client(grpc::CreateChannel(absl::GetFlag(FLAGS_grpc_server_addr), grpc::InsecureChannelCredentials()));
  client.OpenCamera(absl::GetFlag(FLAGS_amcrest_connection_file));
  // Start recording
  // Stop recording
  // Get state
  // Download file
  return 0;
}
