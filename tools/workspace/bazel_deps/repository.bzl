load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def bazel_deps_repository():
    commit = "f7f68a03982bfcc09faa8cb6e2a1564eacc6106c"
    http_archive(
        name = "com_github_mjbots_bazel_deps",
        url = "https://github.com/mjbots/bazel_deps/archive/{}.zip".format(commit),
        # Try the following empty sha256 hash first, then replace with whatever
        # bazel says it is looking for once it complains.
        sha256 = "356f34a15c75a4e5e91baedcc9972874ca2395c37f656fb33c9a2f816da88e6c",
        strip_prefix = "bazel_deps-{}".format(commit),
    )

