#pragma once

#include <opencv2/core/affine.hpp>
#include <opencv2/core/core.hpp>
#include "Eigen/Core"
#include "Eigen/Geometry"

extern "C" {
#include "apriltag/apriltag.h"
#include "apriltag/apriltag_pose.h"
#include "apriltag/tagStandard41h12.h"
}

#include "witness/api/tracking.pb.h"

namespace witness {
namespace server {
namespace converters {

struct TagData {
  double error;
  unsigned int id;
  Eigen::Quaterniond quaternion;
  Eigen::Vector3d translation;
};

// Returns a normalized quaternion from the input pose data.
const Eigen::Quaterniond get_quaternion(const apriltag_pose_t &pose);

const Eigen::Matrix3d get_rotation_matrix(const apriltag_pose_t &pose);

const Eigen::Vector3d get_translation(const apriltag_pose_t &pose);

std::ostream &operator<<(std::ostream &os, const Eigen::Quaterniond &quat);

void add_tag(const TagData &tag, witness::api::StartAprilTrackingReply *reply);

}  // namespace converters
}  // namespace server
}  // namespace witness
