#include <gflags/gflags.h>
#include <glog/logging.h>

#include "witness/server/server.h"

DEFINE_bool(verbose, false, "Display program name before message");

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetUsageMessage("Some usage message here");
  gflags::SetVersionString(WITNESS_VERSION);
  FLAGS_logtostderr = 1;
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  assert(video_extension_validator_registered);
  assert(photo_extension_validator_registered);
  witness::server::RunServer();
  gflags::ShutDownCommandLineFlags();
  return 0;
}
