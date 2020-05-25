#!/bin/bash
set -e

# Note that this must be ran from within the directory containing this script.
./generate_dockerfiles.sh

TAG="base"

# Build base images
docker build -t murtis/witness_amd64:"$TAG" -f Dockerfile.amd64 .
docker build -t murtis/witness_aarch64:"$TAG" -f Dockerfile.aarch64 .
docker build -t murtis/witness_armv7hf:"$TAG" -f Dockerfile.armv7hf .

# Push base images
docker push murtis/witness_amd64:"$TAG"
docker push murtis/witness_aarch64:"$TAG"
docker push murtis/witness_armv7hf:"$TAG"
