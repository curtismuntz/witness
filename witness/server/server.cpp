#include <cstdint>
#include <memory>
#include <string>

#include "witness/server/file_operations.h"
#include "witness/server/server.h"

static bool ValidateRotation(const char *flagname, int value) {
  if (std::abs(value) > 360) {
    LOG(ERROR) << "Rotation value needs to be within +- 360";
    return false;
  }
  return true;
}

DEFINE_string(media_dir, "/data", "Directory to store media");
DEFINE_string(photo_extension, ".jpg", "Photo extension");
DEFINE_string(video_extension, ".avi", "Video extension");
DEFINE_int32(camera_rotation, 0, "Camera rotation in degrees");
DEFINE_validator(photo_extension, &witness::server::file_operations::ValidateExtension);
DEFINE_validator(video_extension, &witness::server::file_operations::ValidateExtension);
DEFINE_validator(camera_rotation, &ValidateRotation);

namespace witness {
namespace server {

WitnessService::WitnessService() : webcam_(FLAGS_camera_rotation) {
  LOG(INFO) << "Witness server version " << WITNESS_VERSION;
}

Status WitnessService::StartRecording(ServerContext *context, const StartRecordingRequest *request,
                                      StartRecordingReply *reply) {
  LOG(INFO) << "StartRecording requested" << std::endl;
  auto fname = witness::server::file_operations::DecideFilename(
      FLAGS_media_dir, request->filename(), FLAGS_video_extension);

  if (!webcam_.IsActive()) {
    auto success = webcam_.StartRecording(fname);
    if (!success) {
      auto reply_error = reply->mutable_error();
      reply_error->set_code(Error::UNKNOWN);
      reply_error->set_message("Issue starting recording...");
    }
  } else {
    auto reply_error = reply->mutable_error();
    reply_error->set_code(Error::CAMERA_ACTIVE);
    reply_error->set_message("Camera is already active.");
  }

  return Status::OK;
}

Status WitnessService::StartTimelapse(ServerContext *context, const StartTimelapseRequest *request,
                                      StartTimelapseReply *reply) {
  LOG(INFO) << "StartTimelapse requested" << std::endl;

  // cut a dir with filename
  auto folder_name = file_operations::DecideFilename(FLAGS_media_dir, request->filename(), "");

  LOG(INFO) << "Making Directory " << folder_name;
  file_operations::MakeDir(folder_name);

  // auto fname = witness::server::file_operations::DecideFilename(
  // FLAGS_media_dir, request->filename(), FLAGS_video_extension);
  if (!webcam_.IsActive()) {
    auto success = webcam_.StartTimelapse(folder_name, FLAGS_video_extension, FLAGS_photo_extension,
                                          request->sleep_for());
    if (!success) {
      auto reply_error = reply->mutable_error();
      reply_error->set_code(Error::UNKNOWN);
      reply_error->set_message("Issue starting recording...");
    }
  } else {
    auto reply_error = reply->mutable_error();
    reply_error->set_code(Error::CAMERA_ACTIVE);
    reply_error->set_message("Camera is already active.");
  }

  return Status::OK;
}

Status WitnessService::StopRecording(ServerContext *context, const StopRecordingRequest *request,
                                     StopRecordingReply *reply) {
  LOG(INFO) << "StopRecording requested" << std::endl;
  auto success = webcam_.StopRecording();

  if (!success) {
    auto reply_error = reply->mutable_error();
    auto msg = "Issue stopping recording...";
    reply_error->set_code(Error::UNKNOWN);
    reply_error->set_message(msg);
    LOG(INFO) << msg;
  }

  return Status::OK;
}

Status WitnessService::ClearData(ServerContext *context, const ClearDataRequest *request,
                                 ClearDataReply *reply) {
  LOG(INFO) << "ClearData requested" << std::endl;
  witness::server::file_operations::ClearDir(FLAGS_media_dir);
  return Status::OK;
}

Status WitnessService::StartMonitor(ServerContext *context, const StartMonitorRequest *request,
                                    StartMonitorReply *reply) {
  LOG(INFO) << "StartMonitor requested" << std::endl;
  auto fname = witness::server::file_operations::DecideFilename(
      FLAGS_media_dir, std::string{"monitor"}, FLAGS_video_extension);
  if (!webcam_.IsActive()) {
    auto success = webcam_.StartMonitoring(fname);
    if (!success) {
      auto reply_error = reply->mutable_error();
      reply_error->set_code(Error::UNKNOWN);
      reply_error->set_message("Issue starting recording...");
    }
  } else {
    auto reply_error = reply->mutable_error();
    reply_error->set_code(Error::CAMERA_ACTIVE);
    reply_error->set_message("Camera is already active.");
  }
  return Status::OK;
}

Status WitnessService::GetServerState(ServerContext *context, const ServerStateRequest *request,
                                      ServerStateReply *reply) {
  LOG(INFO) << "GetServerState requested" << std::endl;

  auto reply_message = reply->mutable_data();
  auto state = reply_message->mutable_state();
  if (webcam_.IsMonitoring()) {
    LOG(INFO) << "monitoring";
    state->set_state(ServerState::MONITORING);
  } else if (webcam_.IsRecording()) {
    LOG(INFO) << "recording";
    state->set_state(ServerState::RECORDING);
  } else {
    LOG(INFO) << "idle";
    state->set_state(ServerState::IDLE);
  }
  return Status::OK;
}

Status WitnessService::TakePhoto(ServerContext *context, const TakePhotoRequest *request,
                                 TakePhotoReply *reply) {
  LOG(INFO) << "Take Photo Requested";
  auto fname = witness::server::file_operations::DecideFilename(
      FLAGS_media_dir, request->filename(), FLAGS_photo_extension);
  auto success = webcam_.SaveImage(fname);
  if (success) {
    LOG(INFO) << "Saved file " << fname << std::endl;
    auto reply_message = reply->mutable_data();
    reply_message->set_filename(fname);
  } else {
    auto reply_error = reply->mutable_error();
    reply_error->set_code(Error::UNKNOWN);
    reply_error->set_message("Unable to save image");
  }
  return Status::OK;
}

Status WitnessService::OpenWebcam(ServerContext *context, const OpenWebcamRequest *request,
                                  OpenWebcamReply *reply) {
  auto opened = webcam_.OpenCamera(request->webcam_id());
  if (!opened) {
    auto reply_error = reply->mutable_error();
    reply_error->set_code(Error::UNKNOWN);
    reply_error->set_message("Unable to open device");
  }
  return Status::OK;
}

Status WitnessService::GetFileList(ServerContext *context, const GetFileListRequest *request,
                                   ServerWriter<FileListReply> *writer) {
  auto file_list = witness::server::file_operations::ListDir(FLAGS_media_dir, FLAGS_photo_extension,
                                                             FLAGS_video_extension);
  for (const auto str : file_list) {
    FileListReply flr;
    flr.mutable_data()->set_filename(str);
    writer->Write(flr);
  }
  return Status::OK;
}

Status WitnessService::GetServerVersion(ServerContext *context, const VersionRequest *request,
                                        VersionReply *reply) {
  LOG(INFO) << "GetServerVersion requested" << std::endl;
  LOG(INFO) << "Witness server version " << WITNESS_VERSION;
  reply->set_version(WITNESS_VERSION);
  return Status::OK;
}

Status WitnessService::SetCameraRotation(ServerContext *context,
                                         const CameraRotationRequest *request,
                                         CameraRotationReply *reply) {
  auto degrees = request->degrees();
  LOG(INFO) << "CameraRotation of " << degrees << " requested" << std::endl;
  webcam_.SetCameraRotation(degrees);
  return Status::OK;
}

Status WitnessService::StartAprilTracking(ServerContext *context,
                                          const StartAprilTrackingRequest *request,
                                          StartAprilTrackingReply *reply) {
  auto tag_id = request->apriltag_id();
  LOG(INFO) << "Using april tag id: " << tag_id;
  webcam_.StartAprilTracking(tag_id);
  return Status::OK;
}

void RunServer() {
  const std::string kServerAddress{"0.0.0.0:50051"};
  WitnessService service;
  ServerBuilder builder;
  builder.AddListeningPort(kServerAddress, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << kServerAddress << std::endl;
  server->Wait();
}
}  // namespace server
}  // namespace witness
