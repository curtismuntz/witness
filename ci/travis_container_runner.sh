#! /usr/bin/env bash

cmd="$@"

docker run \
  --rm \
  -it \
  -v "$PWD":/opt/src \
  murtis/bazelisk_container ${cmd}
