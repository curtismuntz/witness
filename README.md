# witness
[![Build Status](https://travis-ci.org/curtismuntz/witness.svg?branch=master)](https://travis-ci.org/curtismuntz/witness)

an api callable webcam project.

See the [protobuf definition](witness/api/witness.proto) for supported features and more information.

# usage

Run server locally:

`bazel run witness/server:server -- -media_dir /tmp`

Run server in a container:

```
bazel run witness/deploy:container
docker run --rm -it --device=/dev/video1:/dev/video0:rwm bazel/witness/deploy:container
```

Run simple client gui:

`bazel run witness/client:simple_gui -c opt`
