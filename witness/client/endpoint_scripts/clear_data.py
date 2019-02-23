#!/usr/bin/env python3

import common
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


def clear_data(service_stub):
    clear_request = witness_pb2.ClearDataRequest()
    reply = service_stub.ClearData(clear_request, DEFAULT_TIMEOUT)
    print(reply)


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, _ = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    clear_data(service_stub)
