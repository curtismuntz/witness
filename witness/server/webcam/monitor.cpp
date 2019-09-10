#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/videoio.hpp"

namespace witness {
namespace server {
namespace webcam_manager {

// Entrypoint for monitoring. Assumes that the camera ID is not active.
// It is up to the server to decide if this is ran. This
class Monitor {
 public:
  Monitor(std::shared_ptr<witness::server::webcam> cam);
  ~Monitor();

  bool Start(const std::string &fname) {
    if camera_->IsActive() {
      return false;
    }
    camera_->Claim();
    worker_thread_ =
        std::make_unique<std::thread>(&Monitor::Loop, this, fname);
    return true;
  };

  bool Stop() {
    monitoring_ = false;
    worker_thread_->join();
    worker_thread_->reset(nullptr);
    camera_->Unclaim();
    camera_->Close();
  };
 private:
  void Loop(const std::string &fname);
  bool monitoring_;
  std::shared_ptr<witness::server::webcam> camera_;
  std::unique_ptr<std::thread> worker_thread_;
};

}  // namespace webcam_manager
}  // namespace server
}  // namespace witness
