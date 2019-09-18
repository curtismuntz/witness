#!/bin/bash

modprobe bcm2835-v4l2
insmod bcm2835-v4l2
echo "You may need to run the following command on the resin host"
echo "modprobe bcm2835-v4l2"

/opt/witness/witness/server/server
