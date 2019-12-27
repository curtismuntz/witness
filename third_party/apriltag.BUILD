# AprilTag is a visual fiducial system popular for robotics research.
# https://april.eecs.umich.edu/software/apriltag

licenses(["notice"])  #BSD-2-Clause

cc_library(
    name = "apriltag",
    hdrs = [
        "apriltag.h",
        "apriltag_pose.h",
        "tagStandard41h12.h",
        "tag36h11.h"
    ],
    include_prefix = "apriltag",
    visibility = ["//visibility:public"],
    deps = [
        ":common",
        ":tags",
    ],
)

cc_library(
    name = "common",
    # visibility = ["//visibility:public"],
    srcs = glob(["common/*.c"]),
    hdrs = glob(["common/*.h"]),
)

cc_library(
    name = "tags",
    srcs = glob([
        "apriltag*.c",
        "tagStandard41h12.c",
        "tag36h11.c",
    ]),
    hdrs = glob([
        "apriltag*.h",
        "tag36h11.h",
    ]),
    deps = [":common"],
)
