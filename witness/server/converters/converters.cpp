#include "witness/server/converters/converters.h"
#include "witness/server/time/time.h"

namespace witness::server::converters {

const Eigen::Quaterniond get_quaternion(const apriltag_pose_t &pose) {
  auto rot = get_rotation_matrix(pose);
  Eigen::Quaterniond q(rot);
  return q.normalized();
}

const Eigen::Matrix3d get_rotation_matrix(const apriltag_pose_t &pose) {
  Eigen::Matrix3d rot;
  rot << pose.R->data[0], pose.R->data[1], pose.R->data[2], pose.R->data[3], pose.R->data[4],
      pose.R->data[5], pose.R->data[6], pose.R->data[7], pose.R->data[8];
  return rot;
}

const Eigen::Vector3d get_translation(const apriltag_pose_t &pose) {
  return Eigen::Vector3d(pose.t->data[0], pose.t->data[1], pose.t->data[2]);
}

std::ostream &operator<<(std::ostream &os, const Eigen::Quaterniond &quat) {
  os << "[" << quat.w() << ", " << quat.vec().x() << ", " << quat.vec().y() << ", "
     << quat.vec().z() << "]";
  return os;
}

void add_tag(const TagData &tag, witness::api::StartAprilTrackingReply *reply) {
  auto reply_tag = reply->add_tag();

  auto timepoint = witness::server::time::current_time();
  auto time = reply_tag->mutable_timestamp();
  time->set_seconds(timepoint.first.count());
  time->set_nanos(timepoint.second.count());

  auto vec = reply_tag->mutable_vec();
  vec->set_x(tag.translation.x());
  vec->set_y(tag.translation.y());
  vec->set_z(tag.translation.z());

  auto quat = reply_tag->mutable_quat();
  quat->set_w(tag.quaternion.w());
  auto qvec = quat->mutable_vec();
  qvec->set_x(tag.quaternion.vec().x());
  qvec->set_y(tag.quaternion.vec().y());
  qvec->set_z(tag.quaternion.vec().z());

  reply_tag->set_error(tag.error);
}

}  // namespace witness::server::converters
