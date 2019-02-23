#!/bin/bash
set -e

# Note that this must be ran from within the directory containing this script.
./generate_dockerfiles.sh

# Build base images
docker build -t murtis/witness_amd64:base -f Dockerfile.amd64 .
docker build -t murtis/witness_armv7hf:base -f Dockerfile.armv7hf .

# Push base images
docker push murtis/witness_amd64:base
docker push murtis/witness_armv7hf:base
