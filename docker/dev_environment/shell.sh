#!/usr/bin/env bash

set -e

built_images=$(docker images --filter reference=murtis/witness_dev --format "{{.ID}}")
if [[ -z "$built_images" ]]; then
  ./build.sh
fi

WORKSPACE_ROOT="$( cd "$(dirname "$0")"/../.. ; pwd -P )"
CMD=$*

if [[ -z $CMD ]]; then
  CMD="/bin/bash"
fi

docker run --rm -it \
       -v "$WORKSPACE_ROOT":/root/src \
       -v "$HOME"/.docker/config.json:/root/.docker/config.json \
       -v "$HOME/.cache":/root/.cache \
       -v /var/run/docker.sock:/var/run/docker.sock \
       murtis/witness_dev:latest $CMD
