#! /usr/bin/env bash
set -e

usage() {
  echo "Usage:"
  echo " $0 \$config"
  echo "ie:"
  echo " $0 aarch64"
  echo " $0 armv7hf"
  echo " $0 amd64"
}

ARCHITECTURE_CONFIG=$1
if [[ -z "$ARCHITECTURE_CONFIG" ]]; then
  echo "Architecture configuration needs to be specified"
  usage
  exit 1
fi

bazel run \
  --local_ram_resources=HOST_RAM*.25 \
  --local_cpu_resources=HOST_CPUS-3 \
  --config="$1" \
  "//deploy:push_witness_$1"
