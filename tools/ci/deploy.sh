#!/bin/bash

docker run \
  --rm \
  -it \
  -v "$PWD":/opt/src \
  murtis/bazel bazel run //witness/...
