#! /usr/bin/env bash
cmd="$@"

docker run \
  --rm \
  -it \
  -v "$PWD":/root/src \
  murtis/witness_dev:latest ${cmd}
