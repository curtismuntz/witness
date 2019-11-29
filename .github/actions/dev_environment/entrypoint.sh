#!/usr/bin/env sh
set -ex

WORKING_DIRECTORY="${INPUT_WORKING_DIRECTORY:-./}"
cd "${WORKING_DIRECTORY}";

$@
