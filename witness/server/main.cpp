#include <gflags/gflags.h>
#include <glog/logging.h>

#include "witness/server/server.h"

DEFINE_bool(verbose, false, "Display program name before message");

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetVersionString(WITNESS_VERSION);
  LOG(INFO) << "Hello demo";
  LOG(INFO) << "Hello demo";
  LOG(INFO) << "Hello demo";
  LOG(INFO) << "Hello demo";
  FLAGS_logtostderr = 1;
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  witness::server::RunServer();
  gflags::ShutDownCommandLineFlags();
  return 0;
}
