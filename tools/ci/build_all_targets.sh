#!/bin/bash
set -e

bazel build //witness/...
bazel build //witness/... --config=armv7hf
