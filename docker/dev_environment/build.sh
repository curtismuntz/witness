#!/usr/bin/env bash
set -e
docker build -t murtis/witness_dev:latest .
docker push murtis/witness_dev:latest
