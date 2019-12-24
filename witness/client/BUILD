load("@io_bazel_rules_rust//rust:rust.bzl", "rust_binary")
load("@io_bazel_rules_rust//proto:toolchain.bzl", "GRPC_COMPILE_DEPS")

rust_binary(
    name = "client",
    srcs = ["client.rs"],
    visibility = ["//visibility:public"],
    deps = [
        "//witness/api:rust_grpc",
    ] + GRPC_COMPILE_DEPS,
)
