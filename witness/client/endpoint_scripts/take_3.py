#!/usr/bin/env python3

import time
import common
import take_photo
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


def take_3(service_stub):
    take_photo.take_photo(service_stub)
    time.sleep(1)
    take_photo.take_photo(service_stub)
    time.sleep(1)
    take_photo.take_photo(service_stub)


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, _ = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    take_3(service_stub)
