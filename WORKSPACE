load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
register_toolchains("//:system_installed_python_toolchain")

###############################
# murtis maintained external bazel projects
###############################
http_archive(
    name = "murtis_bazel_tools",
    sha256 = "ac407b2a169426007715d80de4269795676b963a9b5bddefffdf45399ab6010c",
    strip_prefix = "bazel_tools-c3f0499f7aae8229f464f1b0bd802a3fdd6f39c1",
    urls = ["https://github.com/curtismuntz/bazel_tools/archive/c3f0499f7aae8229f464f1b0bd802a3fdd6f39c1.tar.gz"],
)

load("@murtis_bazel_tools//tools:deps.bzl", "linter_dependencies", "google_cpp_dependencies")

linter_dependencies()
google_cpp_dependencies()

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
    sha256 = "413bb1ec0895a8d3249a01edf24b82fd06af3c8633c9fb833a0cb1d4b234d46d",
    strip_prefix = "rules_docker-0.12.0",
    urls = ["https://github.com/bazelbuild/rules_docker/releases/download/v0.12.0/rules_docker-v0.12.0.tar.gz"],
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

load(
    "@io_bazel_rules_docker//cc:image.bzl",
    _cc_image_repos = "repositories",
)

_cc_image_repos()

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
    strip_prefix = "apriltag-3.1.1",
    url = "https://github.com/AprilRobotics/apriltag/archive/3.1.1.tar.gz",
    sha256 = "7349e1fcc8b2979230b46c0d62ccf2ba2bbd611d87ef80cfd37ffe74425f5efb",
)

http_archive(
    name = "cpp_httplib_archive",
    build_file = "//third_party:cpp-httplib.BUILD",
    strip_prefix = "cpp-httplib-0.2.4",
    urls = ["https://github.com/yhirose/cpp-httplib/archive/v0.2.4.tar.gz"],
    sha256 = "463b30ec1ebb1c8c44cf33ea471f079ed14c17bbe93966a00dfde5812888972e",
)
