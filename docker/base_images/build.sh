#!/bin/bash
set -e

# Note that this must be ran from within the directory containing this script.
./generate_dockerfiles.sh

tag="base"

arches=( amd64 aarch64 armv7hf )

for arch in "${arches[@]}"; do
  echo "$arch"
  full_tag=murtis/witness_"$arch":"$tag"
  docker build -t "$full_tag" -f Dockerfile."$arch" .
  docker push "$full_tag"
done

