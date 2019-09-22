#! /usr/bin/env bash

cmd="$@"

docker run \
  --rm \
  -it \
  -v "$PWD":/opt/src \
  murtis/witness_dev:latest ${cmd}
