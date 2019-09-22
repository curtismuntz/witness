#! /usr/bin/env bash
set -e

bazel run --host_force_python=PY2 --local_ram_resources=HOST_RAM*.25 --local_cpu_resources=HOST_CPUS-3 //deploy:push_witness_armv7 --config=armv7hf
bazel run --host_force_python=PY2 --local_ram_resources=HOST_RAM*.25 --local_cpu_resources=HOST_CPUS-3 //deploy:push_witness_aarch64 --config=aarch64
# Disabled because 19.04 compile doesnt work.
#bazel run --host_force_python=PY2 --local_ram_resources=HOST_RAM*.25 --local_cpu_resources=HOST_CPUS-3 //deploy:push_witness_amd64
