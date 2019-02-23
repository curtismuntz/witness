# witness

an api callable webcam project

To test locally:

`BAZEL_PYTHON=/usr/bin/python2.7 bazel run witness/deploy:container -c opt`

`docker run --rm -it --device=/dev/video0:/dev/video0:rwm bazel/witness/deploy:container`

To test cross compile works:

`BAZEL_PYTHON=/usr/bin/python2.7 bazel build witness/deploy:container -c opt --config=rpi`

To deploy to a balena application:

`tools/resin_deploy -t witness/deploy:container -a rpi -r witness_fin -b "-c opt"`
