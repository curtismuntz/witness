load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

###############################
# murtis maintained external bazel projects
###############################
http_archive(
    name = "murtis_bazel_tools",
    sha256 = "f5839e08855386011cf78c4204d993c40527ea5d33f7bb78c53eb6ce94d72ded",
    strip_prefix = "bazel_tools-713720bb61c13868c4f57ed66777a75aa17f6019",
    urls = ["https://github.com/curtismuntz/bazel_tools/archive/713720bb61c13868c4f57ed66777a75aa17f6019.tar.gz"],
)

http_archive(
    name = "com_github_curl",
    build_file = "//third_party:curl.BUILD",
    sha256 = "ffa8f79f68dd77b08987ce16acd1f292875df8ab3bf7e3654f98d62b445ebd9a",
    strip_prefix = "curl-curl-7_78_0",
    urls = ["https://github.com/curl/curl/archive/refs/tags/curl-7_78_0.tar.gz"],
)

load("@murtis_bazel_tools//tools:deps.bzl", "google_cpp_dependencies", "linter_dependencies")

linter_dependencies()

google_cpp_dependencies()

http_archive(
    name = "murtis_bazel_compilers",
    sha256 = "35783cb0bc145721f903132c985cc0beb9910a4d53dfbf656f618fa6f5628d4d",
    strip_prefix = "bazel_compilers-0.5.0",
    urls = ["https://github.com/curtismuntz/bazel_compilers/archive/0.5.0.tar.gz"],
)

load("@murtis_bazel_compilers//compilers:dependencies.bzl", "cross_compiler_dependencies")

cross_compiler_dependencies()

###############################
# protobuf
###############################
http_archive(
    name = "rules_proto",
    sha256 = "c22cfcb3f22a0ae2e684801ea8dfed070ba5bed25e73f73580564f250475e72d",
    strip_prefix = "rules_proto-4.0.0-3.19.2",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0-3.19.2.tar.gz",
    ],
)

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()

http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "",
    strip_prefix = "grpc-1.44.0",
    urls = [
        "https://github.com/grpc/grpc/archive/refs/tags/v1.44.0.tar.gz",
    ],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

# Not mentioned in official docs... mentioned here https://github.com/grpc/grpc/issues/20511
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()

###############################
# Docker
###############################
http_archive(
    name = "io_bazel_rules_docker",
    sha256 = "85ffff62a4c22a74dbd98d05da6cf40f497344b3dbf1e1ab0a37ab2a1a6ca014",
    strip_prefix = "rules_docker-0.23.0",
    urls = ["https://github.com/bazelbuild/rules_docker/releases/download/v0.23.0/rules_docker-v0.23.0.tar.gz"],
)

load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)

container_repositories()

load("@io_bazel_rules_docker//repositories:deps.bzl", container_deps = "deps")

container_deps()

load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_pull",
)

WITNESS_CONTAINER_TAG = "base"

container_pull(
    name = "witness_armv7_docker_base",
    digest = "sha256:4aa2e2289e8b7c586b0e41bf9b889dafc2aad81ff683312776cf99a328f90d0e",
    registry = "index.docker.io",
    repository = "murtis/witness_armv7hf",
    tag = WITNESS_CONTAINER_TAG,
)

container_pull(
    name = "witness_aarch64_docker_base",
    digest = "sha256:02bc8b9cb0fb5e7923f28128b65eb0dcd8f8a268563c413f29d8157494e920ed",
    registry = "index.docker.io",
    repository = "murtis/witness_aarch64",
    tag = WITNESS_CONTAINER_TAG,
)

load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)

container_repositories()

load(
    "@io_bazel_rules_docker//cc:image.bzl",
    _cc_image_repos = "repositories",
)

_cc_image_repos()

###############################
# c++
###############################
#http_archive(
#    name = "opencv",
#    build_file = "//third_party:opencv.BUILD",
#    sha256 = "f3b160b9213dd17aa15ddd45f6fb06017fe205359dbd1f7219aad59c98899f15",
#    strip_prefix = "opencv-3.1.0",
#    url = "https://github.com/opencv/opencv/archive/3.1.0.tar.gz",
#)

http_archive(
    name = "libjpeg_archive",
    build_file = "//third_party:jpeg.BUILD",
    sha256 = "301d99bdb1abbb849975a56689680649ca7d8dbe06dc4bda08edeb1e5e4f77c0",
    strip_prefix = "libjpeg-libjpeg-6b",
    urls = [
        "https://github.com/freedesktop/libjpeg/archive/libjpeg-6b.tar.gz",
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

http_archive(
    name = "apriltag_archive",
    build_file = "//third_party:apriltag.BUILD",
    sha256 = "7349e1fcc8b2979230b46c0d62ccf2ba2bbd611d87ef80cfd37ffe74425f5efb",
    strip_prefix = "apriltag-3.1.1",
    url = "https://github.com/AprilRobotics/apriltag/archive/3.1.1.tar.gz",
)

http_archive(
    name = "cpp_httplib_archive",
    build_file = "//third_party:cpp-httplib.BUILD",
    sha256 = "c8ef7d19842c63747eaa844d25732eb999250716bdf658b876a9c34b274af22c",
    strip_prefix = "cpp-httplib-0.9.7",
    urls = ["https://github.com/yhirose/cpp-httplib/archive/v0.9.7.tar.gz"],
)

http_archive(
    name = "eigen_archive",
    build_file = "//third_party:eigen.BUILD",
    sha256 = "a8d87c8df67b0404e97bcef37faf3b140ba467bc060e2b883192165b319cea8d",
    strip_prefix = "eigen-git-mirror-3.3.7/",
    urls = ["https://github.com/eigenteam/eigen-git-mirror/archive/3.3.7.tar.gz"],
)

http_archive(
    name = "io_bazel_rules_rust",
    sha256 = "b6da34e057a31b8a85e343c732de4af92a762f804fc36b0baa6c001423a70ebc",
    strip_prefix = "rules_rust-55f77017a7f5b08e525ebeab6e11d8896a4499d2",
    urls = [
        # Master branch as of 2019-10-07
        "https://github.com/bazelbuild/rules_rust/archive/55f77017a7f5b08e525ebeab6e11d8896a4499d2.tar.gz",
    ],
)

http_archive(
    name = "com_github_p-ranav_indicators",
    build_file = "//third_party:indicators.BUILD",
    sha256 = "b768f1b7ca64a413503f72d5460cc617c1458c17fb7a8c0ee503d753e1f20d03",
    strip_prefix = "indicators-2.2",
    urls = ["https://github.com/p-ranav/indicators/archive/refs/tags/v2.2.tar.gz"],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "9245b0549e88e356cd6a25bf79f97aa19332083890b7ac6481a2affb6ada9752",
    strip_prefix = "bazel-skylib-1.1.1",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/archive/1.1.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/archive/1.1.1.tar.gz",
    ],
)

load("//tools/workspace:default.bzl", "add_default_repositories")

add_default_repositories()

load(
    "@com_github_mjbots_bazel_deps//tools/workspace:default.bzl",
    bazel_deps_add = "add_default_repositories",
)

bazel_deps_add()
