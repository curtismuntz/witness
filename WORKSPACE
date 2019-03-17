load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

###############################
# murtis maintained external bazel projects
###############################
http_archive(
    name = "murtis_bazel_tools",
    sha256 = "a6bcb86c7b5ed4c55fddd4b64d9c4916677e758f0fdd7f051f9dbfca8a6a8eee",
    strip_prefix = "bazel_tools-811127f89abd954e3ed4a89e9aa90b468c71c2d4",
    urls = ["https://github.com/curtismuntz/bazel_tools/archive/811127f89abd954e3ed4a89e9aa90b468c71c2d4.tar.gz"],
)

load("@murtis_bazel_tools//tools:github_repo.bzl", "github_repo")
load("@murtis_bazel_tools//tools:deps.bzl", "linter_dependencies")

linter_dependencies()

github_repo(
    name = "murtis_bazel_compilers",
    repo = "bazel_compilers",
    sha256 = "6d0f5efaa3ac073906ef3351da4038bfd081296d6307e0e87c33ffc2cc876a4f",
    tag = "eaafbe4ee813b2d1363e8022eee2218ac3e2da06",
    user = "curtismuntz",
)

load("@murtis_bazel_compilers//compilers:dependencies.bzl", "cross_compiler_dependencies")

cross_compiler_dependencies()

###############################
# protobuf
###############################
http_archive(
    name = "build_stack_rules_proto",
    sha256 = "128c4486b1707db917411c6e448849dd76ea3b8ba704f9e0627d9b01f2ee45fe",
    strip_prefix = "rules_proto-f5d6eea6a4528bef3c1d3a44d486b51a214d61c2",
    urls = ["https://github.com/stackb/rules_proto/archive/f5d6eea6a4528bef3c1d3a44d486b51a214d61c2.tar.gz"],
)

load("@build_stack_rules_proto//cpp:deps.bzl", "cpp_grpc_library")

cpp_grpc_library()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

###############################
# Docker
###############################
http_archive(
    name = "io_bazel_rules_docker",
    sha256 = "aed1c249d4ec8f703edddf35cbe9dfaca0b5f5ea6e4cd9e83e99f3b0d1136c3d",
    strip_prefix = "rules_docker-0.7.0",
    urls = ["https://github.com/bazelbuild/rules_docker/archive/v0.7.0.tar.gz"],
)

load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)

container_repositories()

load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_pull",
)

container_pull(
    name = "witness_armv7_docker_base",
    registry = "index.docker.io",
    repository = "murtis/witness_armv7hf",
    tag = "base",
)

container_pull(
    name = "witness_amd64_docker_base",
    registry = "index.docker.io",
    repository = "murtis/witness_amd64",
    tag = "base",
)

###############################
# python
###############################
load("@build_stack_rules_proto//python:deps.bzl", "python_grpc_library")

python_grpc_library()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@io_bazel_rules_python//python:pip.bzl", "pip_import", "pip_repositories")

pip_repositories()

pip_import(
    name = "protobuf_py_deps",
    requirements = "@build_stack_rules_proto//python/requirements:protobuf.txt",
)

load("@protobuf_py_deps//:requirements.bzl", protobuf_pip_install = "pip_install")

protobuf_pip_install()

pip_import(
    name = "grpc_py_deps",
    requirements = "@build_stack_rules_proto//python:requirements.txt",
)

load("@grpc_py_deps//:requirements.bzl", grpc_pip_install = "pip_install")

grpc_pip_install()

# already included by another project somehow...
# http_archive(
#     name = "subpar",
#     strip_prefix = "subpar-a4f9b23bf01bcc7a52d458910af65a90ee991aff",
#     urls = ["https://github.com/google/subpar/archive/a4f9b23bf01bcc7a52d458910af65a90ee991aff.tar.gz"],
# )

###############################
# c++
###############################
http_archive(
    name = "opencv_archive",
    build_file = "//third_party:opencv.BUILD",
    sha256 = "f3b160b9213dd17aa15ddd45f6fb06017fe205359dbd1f7219aad59c98899f15",
    strip_prefix = "opencv-3.1.0",
    url = "https://github.com/opencv/opencv/archive/3.1.0.tar.gz",
)

http_archive(
    name = "libjpeg_archive",
    build_file = "//third_party:jpeg.BUILD",
    sha256 = "75c3ec241e9996504fe02a9ed4d12f16b74ade713972f3db9e65ce95cd27e35d",
    strip_prefix = "jpeg-6b",
    urls = [
        "https://svwh.dl.sourceforge.net/project/libjpeg/libjpeg/6b/jpegsrc.v6b.tar.gz",
    ],
)

http_archive(
    name = "libpng_archive",
    build_file = "//third_party:png.BUILD",
    sha256 = "e30bf36cd5882e017c23a5c6a79a9aa1a744dd5841bb45ff7035ec6e3b3096b8",
    strip_prefix = "libpng-1.6.29",
    url = "http://download.sourceforge.net/libpng/libpng-1.6.29.tar.gz",
)

bind(
    name = "png",
    actual = "@libpng_archive//:libpng",
)

http_archive(
    name = "zlib_git",
    build_file = "//third_party:zlib.BUILD",
    sha256 = "e380bd1bdb6447508beaa50efc653fe45f4edc1dafe11a251ae093e0ee97db9a",
    strip_prefix = "zlib-1.2.8",
    url = "https://github.com/madler/zlib/archive/v1.2.8.tar.gz",
)

bind(
    name = "zlib",
    actual = "@zlib_git//:zlib",
)

#Import the glog files. gflags and gtest gets pulled in by grpc somehow.
http_archive(
    name = "com_github_glog_glog",
    build_file = "//third_party:glog.BUILD",
    sha256 = "254da80e5789aecd4a46a8b29c1ff693827d0aa3e06773c8def626582fc8bb82",
    strip_prefix = "glog-7ffca211fe8bf30453da9e27b66000d3735f96b9",
    urls = ["https://github.com/google/glog/archive/7ffca211fe8bf30453da9e27b66000d3735f96b9.tar.gz"],
)

http_archive(
    name = "com_google_absl",
    url = "https://github.com/abseil/abseil-cpp/archive/3e2e9b5557e76d098de4b8a2a659125b98ca519b.tar.gz",
)

# already included by another project somehow...
# http_archive(
#     name = "gtest",
#     url = "https://github.com/google/googletest/archive/release-1.8.0.zip",
#     build_file = "//third_party:gtest.BUILD",
#     strip_prefix = "googletest-release-1.8.0/googletest",
# )

###############################
# Buildifier
# This section allows buildifier to exist as part of the bazel project. Simply run the following
# command to run buildifier on all bazel BUILD/starlark targets.
# bazel run //:buildifier
###############################
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# buildifier is written in Go and hence needs rules_go to be built.
# See https://github.com/bazelbuild/rules_go for the up to date setup instructions.
http_archive(
    name = "io_bazel_rules_go",
    sha256 = "ade51a315fa17347e5c31201fdc55aa5ffb913377aa315dceb56ee9725e620ee",
    url = "https://github.com/bazelbuild/rules_go/releases/download/0.16.6/rules_go-0.16.6.tar.gz",
)

http_archive(
    name = "com_github_bazelbuild_buildtools",
    sha256 = "e8792ae37bfa82eb4efa3e2d93a5b4dcc43d681d13f6d00f183d2ef34a4bc828",
    strip_prefix = "buildtools-4bcdbd1064fcc48180fa30400e39f7a940fdb8f9",
    url = "https://github.com/bazelbuild/buildtools/archive/4bcdbd1064fcc48180fa30400e39f7a940fdb8f9.zip",
)

load("@io_bazel_rules_go//go:def.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@com_github_bazelbuild_buildtools//buildifier:deps.bzl", "buildifier_dependencies")

go_rules_dependencies()

go_register_toolchains()

buildifier_dependencies()
