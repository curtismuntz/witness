FROM ubuntu:xenial
RUN apt update && apt install -y \
                  build-essential \
                  git \
                  python3 \
                  python3-pip \
                  python \
                  python-pip \
                  wget

RUN wget --directory-prefix=/tmp https://github.com/bazelbuild/bazelisk/releases/download/v1.0/bazelisk-linux-amd64 && \
    mv /tmp/bazelisk-linux-amd64 /usr/bin/bazel && \
    chmod +x /usr/bin/bazel

RUN wget -P /tmp https://download.docker.com/linux/static/stable/x86_64/docker-19.03.2.tgz && \
    tar -xf /tmp/docker-19.03.2.tgz -C /tmp && \
    cp /tmp/docker/docker /usr/bin/docker && \
    rm -rf /tmp/docker*

RUN mkdir -p /root/src
WORKDIR /root/src
ENTRYPOINT ["/bin/bash"]
