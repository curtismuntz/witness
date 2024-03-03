#!/usr/bin/env bash
set -e

full_tag=murtis/witness_dev:latest
docker build -t "$full_tag" .
#docker push "$full_tag"
