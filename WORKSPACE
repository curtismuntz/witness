load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

###############################
# murtis maintained external bazel projects
###############################
http_archive(
    name = "murtis_bazel_tools",
    sha256 = "d4509b7646e64382588d71b794af9f8892c8584fa5865837b3f690362a19d995",
    strip_prefix = "bazel_tools-179df94e7c050d69026175d8b6d3a3eab17446cd",
    urls = ["https://github.com/curtismuntz/bazel_tools/archive/179df94e7c050d69026175d8b6d3a3eab17446cd.tar.gz"],
)

load("@murtis_bazel_tools//tools:deps.bzl", "linter_dependencies")

linter_dependencies()

http_archive(
    name = "murtis_bazel_compilers",
    sha256 = "16865fc175a3f64f5179c484d47b80170e7635093348ce51743c1eb261413246",
    strip_prefix = "bazel_compilers-0.4.0",
    urls = ["https://github.com/curtismuntz/bazel_compilers/archive/v0.4.0.tar.gz"],
)

load("@murtis_bazel_compilers//compilers:dependencies.bzl", "cross_compiler_dependencies")

cross_compiler_dependencies()

###############################
# protobuf
###############################
http_archive(
    name = "build_stack_rules_proto",
    sha256 = "85ccc69a964a9fe3859b1190a7c8246af2a4ead037ee82247378464276d4262a",
    strip_prefix = "rules_proto-d9a123032f8436dbc34069cfc3207f2810a494ee",
    urls = ["https://github.com/stackb/rules_proto/archive/d9a123032f8436dbc34069cfc3207f2810a494ee.tar.gz"],
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
    sha256 = "9ff889216e28c918811b77999257d4ac001c26c1f7c7fb17a79bc28abf74182e",
    strip_prefix = "rules_docker-0.10.1",
    urls = ["https://github.com/bazelbuild/rules_docker/archive/v0.10.1.tar.gz"],
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
    name = "witness_aarch64_docker_base",
    registry = "index.docker.io",
    repository = "murtis/witness_aarch64",
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
    sha256 = "ec4a7a3d256ee0a192334644839f00dfdce78949cfdeba673b7339982e573db6",
    strip_prefix = "glog-4cc89c9e2b452db579397887c37f302fb28f6ca1",
    urls = ["https://github.com/google/glog/archive/4cc89c9e2b452db579397887c37f302fb28f6ca1.tar.gz"],
)

http_archive(
    name = "com_google_absl",
    url = "https://github.com/abseil/abseil-cpp/archive/3e2e9b5557e76d098de4b8a2a659125b98ca519b.tar.gz",
)

http_archive(
    name = "apriltag_archive",
    build_file = "//third_party:apriltag.BUILD",
    strip_prefix = "apriltag-3.1.1",
    url = "https://github.com/AprilRobotics/apriltag/archive/3.1.1.tar.gz",
)

http_archive(
    name = "cpp_httplib_archive",
    build_file = "//third_party:cpp-httplib.BUILD",
    strip_prefix = "cpp-httplib-0.2.4",
    urls = ["https://github.com/yhirose/cpp-httplib/archive/v0.2.4.tar.gz"],
    sha256 = "463b30ec1ebb1c8c44cf33ea471f079ed14c17bbe93966a00dfde5812888972e",
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
