# AprilTag is a visual fiducial system popular for robotics research.
# https://april.eecs.umich.edu/software/apriltag

licenses(["notice"])  #BSD-2-Clause

cc_library(
    name = "apriltag",
    hdrs = [
        "apriltag.h",
        "tagStandard41h12.h",
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
    srcs = glob(["common/*.c"]),
    hdrs = glob(["common/*.h"]),
)

cc_library(
    name = "tags",
    srcs = glob([
        "apriltag*.c",
        "tagStandard41h12.c",
    ]),
    hdrs = glob([
        "apriltag*.h",
        "tagStandard41h12.h",
    ]),
    deps = [":common"],
)
