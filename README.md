# Witness: An API callable webcam project.
[![Actions Status](https://github.com/curtismuntz/witness/workflows/ci/badge.svg)](https://github.com/curtismuntz/witness/actions)

An api callable webcam project. Read about it in my blog post on [https://murt.is/articles/2019-03/witness.html](https://murt.is/articles/2019-03/witness.html).

I work a lot in robotics. Sometimes testing robots outside of simulators can result in hardware damaging incidents. Debugging these incidents is a combination of digging through log files and relying on witness accounts. Witnesses are imperfect, as they can be biased or perhaps they see the whole incident.

Typically, witness cameras are used to combat this problem. It becomes part of the operational practice to always start a camera recording when you start a test. The problem with recording every test, is if it’s not automated, eventually you’ll forget to do it. This led me to develop the concept of an unbiased robotic witness. The idea is to have an API callable webcam that is scalable to however many witnesses I want to have.

Using this project, you can now integrate the API calls into your usual robotics testing procedures. Any time you're about to test a new feature, the witnesses can be told to start recording sessions with the same filename (the software prepends hostname to filename). Then, all witness accounts can be downloaded upon test completion. This automates the process of synchronizing tests with witness accounts, as well as ensures a video is always recorded.

# Usage

See the [protobuf definition](witness/api/witness.proto) for supported features and more information.


# Running the service on x86 (local builds)

Run server locally:

`bazel run //witness/server:server -- -media_dir /tmp`

Run server in a container:

```
bazel run //deploy:container_amd64
docker run --rm -it --device=/dev/video1:/dev/video0:rwm bazel/deploy:container_amd64
```

# Running the service on an IoT device

This project has tooling to develop aarch64 and armv7hf containers for use on Raspberry Pi and Nvidia Jetson (Nano, TX1, TX2, and Xavier) computers. These containers can be pushed to dockerhub and pulled onto each device, or you could use [balena](https://balena.io) to manage a fleet of witnesses.

Their structure follows:

* murtis/witness_armv7hf -> works on raspberrypi products
* murtis/witness_aarch64 -> works on nvidia jetson products

Each witness architecture has two tags:

* base -> the base image that bazel pulls down to apply layers on top (see //docker/base_images)
* service -> the compiled service layer that sits on top of the base layer

Build them for a device:

```
# Note, you could push via the run command using the :push_witness_aarch64 target
bazel build --config=aarch64 //deploy:container_aarch64
```

Run them on a device:

```
docker run --rm -it --network=host --device=/dev/video0:/dev/video0:rwm murtis/witness_aarch64:service
```

# Example clients

Run simple cli client:

`bazel run witness/client/endpoint_scripts:shell`
