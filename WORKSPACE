load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

register_toolchains("//:system_installed_python_toolchain")

###############################
# murtis maintained external bazel projects
###############################
http_archive(
    name = "murtis_bazel_tools",
    sha256 = "1123eb08463f5a1a76e873d8c249a746caae89b6c31e8e43b045ff6cdf313821",
    strip_prefix = "bazel_tools-fb5b9ad88abe259e6a2306503870f57154bf44ec",
    urls = ["https://github.com/curtismuntz/bazel_tools/archive/fb5b9ad88abe259e6a2306503870f57154bf44ec.tar.gz"],
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
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_proto_grpc",
    sha256 = "5f0f2fc0199810c65a2de148a52ba0aff14d631d4e8202f41aff6a9d590a471b",
    strip_prefix = "rules_proto_grpc-1.0.2",
    urls = ["https://github.com/rules-proto-grpc/rules_proto_grpc/archive/1.0.2.tar.gz"],
)

load("@rules_proto_grpc//:repositories.bzl", "rules_proto_grpc_repos", "rules_proto_grpc_toolchains")

rules_proto_grpc_toolchains()

rules_proto_grpc_repos()

load("@rules_proto_grpc//cpp:repositories.bzl", rules_proto_grpc_cpp_repos = "cpp_repos")

rules_proto_grpc_cpp_repos()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

###############################
# protobuf (python)
###############################
load("@rules_proto_grpc//python:repositories.bzl", rules_proto_grpc_python_repos = "python_repos")

rules_proto_grpc_python_repos()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@rules_python//python:repositories.bzl", "py_repositories")

py_repositories()

load("@rules_python//python:pip.bzl", "pip_repositories")

pip_repositories()

load("@rules_python//python:pip.bzl", "pip_import")

pip_import(
    name = "rules_proto_grpc_py2_deps",
    python_interpreter = "python",  # Replace this with the platform specific Python 2 name, or remove if not using Python 2
    requirements = "@rules_proto_grpc//python:requirements.txt",
)

load("@rules_proto_grpc_py2_deps//:requirements.bzl", pip2_install = "pip_install")

pip2_install()

pip_import(
    name = "rules_proto_grpc_py3_deps",
    python_interpreter = "python3",
    requirements = "@rules_proto_grpc//python:requirements.txt",
)

load("@rules_proto_grpc_py3_deps//:requirements.bzl", pip3_install = "pip_install")

pip3_install()

###############################
# Docker
###############################
http_archive(
    name = "io_bazel_rules_docker",
    sha256 = "dc97fccceacd4c6be14e800b2a00693d5e8d07f69ee187babfd04a80a9f8e250",
    strip_prefix = "rules_docker-0.14.1",
    urls = ["https://github.com/bazelbuild/rules_docker/releases/download/v0.14.1/rules_docker-v0.14.1.tar.gz"],
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
    registry = "index.docker.io",
    repository = "murtis/witness_armv7hf",
    tag = WITNESS_CONTAINER_TAG,
)

container_pull(
    name = "witness_aarch64_docker_base",
    registry = "index.docker.io",
    repository = "murtis/witness_aarch64",
    tag = WITNESS_CONTAINER_TAG,
)

container_pull(
    name = "witness_amd64_docker_base",
    registry = "index.docker.io",
    repository = "murtis/witness_amd64",
    tag = WITNESS_CONTAINER_TAG,
)

load(
    "@io_bazel_rules_docker//cc:image.bzl",
    _cc_image_repos = "repositories",
)

_cc_image_repos()

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
    sha256 = "35bcc6a3f9612feb92b2153c5e56389ccc1ab46c7ba8781b873a5c2e249eb610",
    strip_prefix = "cpp-httplib-0.6.6",
    urls = ["https://github.com/yhirose/cpp-httplib/archive/v0.6.6.tar.gz"],
)

http_archive(
    name = "eigen_archive",
    build_file = "//third_party:eigen.BUILD",
    sha256 = "a8d87c8df67b0404e97bcef37faf3b140ba467bc060e2b883192165b319cea8d",
    strip_prefix = "eigen-git-mirror-3.3.7/",
    urls = ["https://github.com/eigenteam/eigen-git-mirror/archive/3.3.7.tar.gz"],
)
