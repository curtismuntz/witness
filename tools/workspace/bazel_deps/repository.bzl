load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def bazel_deps_repository():
    commit = "aa535e43e64371aee713badac0e93dd781ee720d"
    http_archive(
        name = "com_github_mjbots_bazel_deps",
        url = "https://github.com/mjbots/bazel_deps/archive/{}.zip".format(commit),
        # Try the following empty sha256 hash first, then replace with whatever
        # bazel says it is looking for once it complains.
        sha256 = "ac7c64007f21de3782f045d8b389bec9bb488fd2465cc21c31cc0f16fdf71906",
        strip_prefix = "bazel_deps-{}".format(commit),
    )

