#!/usr/bin/env bash

docker build -t murtis/witness_dev:latest .
docker push murtis/witness_dev:latest
