#include <gflags/gflags.h>
#include <glog/logging.h>
#include <httplib.h>

#include "witness/server/server.h"

DEFINE_bool(verbose, false, "Display program name before message");
DEFINE_string(media_dir, "/data", "Directory to store media");

void thread_loop() {
  httplib::Server svr;
  svr.set_base_dir(FLAGS_media_dir.c_str());
  svr.listen("0.0.0.0", 8000);
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  gflags::SetVersionString(WITNESS_VERSION);
  FLAGS_logtostderr = 1;
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::thread file_server(&thread_loop);

  witness::server::RunServer(FLAGS_media_dir);
  gflags::ShutDownCommandLineFlags();
  return 0;
}
