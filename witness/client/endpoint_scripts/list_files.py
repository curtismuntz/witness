#!/usr/bin/env python3
import common
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


def list_files(service_stub, quiet=False):
    list_request = witness_pb2.GetFileListRequest()
    files = []
    for reply in service_stub.GetFileList(list_request, DEFAULT_TIMEOUT):
        if not quiet:
            print(reply)
        files.append(reply.data.filename)
    return files


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, _ = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    list_files(service_stub)
