load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

###############################
# murtis maintained external bazel projects
###############################
load("//tools:github_repo.bzl", "github_repo")

github_repo(
    name = "murtis_bazel_compilers",
    repo = "bazel_compilers",
    sha256 = "3dcaa2f1a1677ca99c56a3fa6c458592d8c8b74bac240fe9e57b1008a47b2494",
    tag = "b835dbb8cedd6284501c5d2323352409add8848b",
    user = "curtismuntz",
)

load("@murtis_bazel_compilers//compilers:dependencies.bzl", "cross_compiler_dependencies")

cross_compiler_dependencies()

github_repo(
    name = "murtis_bazel_tools",
    repo = "bazel_tools",
    sha256 = "b1a6caa5151af02b8ac174d7bff3b317f9be347116113fb10abd06feced50826",
    tag = "5d256b9b8bb74a91e8e4370781be9b92910f7b14",
    user = "curtismuntz",
)

load("@murtis_bazel_tools//tools:deps.bzl", "linter_dependencies")

linter_dependencies()

###############################
# protobuf
###############################
http_archive(
    name = "build_stack_rules_proto",
    sha256 = "775981425f2e2313e30ca67b75cac35134df65c5fae2231820d63eec708c65eb",
    strip_prefix = "rules_proto-b74e93b3a197401da858423d2758aaf4f38be4f9",
    urls = ["https://github.com/stackb/rules_proto/archive/b74e93b3a197401da858423d2758aaf4f38be4f9.tar.gz"],
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

http_archive(
    name = "subpar",
    sha256 = "eddbfc920e9cd565500370114316757848b672deba06dc2336acfa81b4ac0e6d",
    strip_prefix = "subpar-1.3.0",
    urls = ["https://github.com/google/subpar/archive/1.3.0.tar.gz"],
)

###############################
# c++
###############################
# cpplint from google style guide
new_git_repository(
    name = "google_styleguide",
    build_file = "//third_party:google_styleguide.BUILD",
    commit = "159b4c81bbca97a9ca00f1195a37174388398a67",
    remote = "https://github.com/google/styleguide.git",
)

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
    sha256 = "f87fa87475ea107b3c69196f39c82b7bbf58fe27c62a338684c20ca17d1d8613",
    url = "https://github.com/bazelbuild/rules_go/releases/download/0.16.2/rules_go-0.16.2.tar.gz",
)

http_archive(
    name = "com_github_bazelbuild_buildtools",
    strip_prefix = "buildtools-9f8fdb20dd423621ef00ced33dcb40204703c2c8",
    url = "https://github.com/bazelbuild/buildtools/archive/9f8fdb20dd423621ef00ced33dcb40204703c2c8.zip",
)

load("@io_bazel_rules_go//go:def.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@com_github_bazelbuild_buildtools//buildifier:deps.bzl", "buildifier_dependencies")

go_rules_dependencies()

go_register_toolchains()

buildifier_dependencies()
