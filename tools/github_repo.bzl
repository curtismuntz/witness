load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def github_repo(name, user, repo, tag, sha256):
    github_url = "https://github.com/" + user + "/" + repo + "/archive/" + tag + ".tar.gz"
    github_prefix = repo + "-" + tag
    if name not in native.existing_rules():
        http_archive(
            name = name,
            url = github_url,
            sha256 = sha256,
            strip_prefix = github_prefix,
        )
