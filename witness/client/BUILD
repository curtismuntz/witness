cc_binary(
    name = "client",
    srcs = ["main.cpp"],
    deps = ["//witness/api:cc_proto",
                    "@com_google_absl//absl/flags:flag",
                    "@com_google_absl//absl/flags:parse",
                    "@com_github_grpc_grpc//:grpc++",
                    "//witness/drivers:amcrest",        "@com_github_google_glog//:glog",        "//witness/api:cc_grpc",
            ],
)
