#!/usr/bin/env python3

import common
import time
import start_timelapse
import stop_recording
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


def calculate_timelapse_sleep(real_life_time, clip_time):
    potential_frames = real_life_time * 30  # seconds * frames per second
    print("Potential frames: %d" % potential_frames)
    clip_frames = clip_time * 30  # seconds * frames per second
    print("Clip frames: %d" % clip_frames)

    if clip_frames > potential_frames:
        print("Timelapse will be shorter than expected...")
    sleep_time = (potential_frames / clip_frames) / 30
    print("Sleeping %is in between frames" % sleep_time)
    return sleep_time


def automated_timelapse(channel):
    real_time = int(input("Real time (s)"))
    clip_time = int(input("Clip time (s)"))
    print("Real time: %i", real_time)
    print("Clip time: %i", clip_time)
    sleep_time = calculate_timelapse_sleep(real_time, clip_time)
    start_timelapse.start_timelapse(channel, int(sleep_time))
    try:
        time.sleep(real_time)
    except KeyboardInterrupt:
        print("early exit requested")
    stop_recording.stop_recording(channel)


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, _ = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    automated_timelapse(service_stub)
