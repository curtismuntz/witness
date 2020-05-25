#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <thread>

#include "witness/server/webcam/webcam.h"

namespace witness::server::webcam::actions {

class WebcamAction {
 public:
  explicit WebcamAction(std::shared_ptr<witness::webcam::Webcam> webcam)
      : webcam_(webcam), working_(false), worker_(nullptr) {}

  bool StartBlocking();
  bool Start();
  bool Stop();
  virtual bool Loop() = 0;

 protected:
  std::shared_ptr<witness::webcam::Webcam> webcam_;
  bool working_;

 private:
  std::unique_ptr<std::thread> worker_;
};

}  // namespace witness::server::webcam::actions
