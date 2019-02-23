#!/bin/bash

docker run \
  --rm \
  -it \
  -v "$PWD":/opt/src \
  murtis/bazel bazel build //witness/... && bazel build //witness/... --config=armv7hf
