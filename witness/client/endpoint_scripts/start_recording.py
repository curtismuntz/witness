#!/usr/bin/env python3

# import witness.client.endpoint_scripts.common as common
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


# def start_recording(service_stub, fname=""):
#     start_request = witness_pb2.StartRecordingRequest()
#     start_reply = service_stub.StartRecording(start_request, DEFAULT_TIMEOUT)
#     print(start_reply)


if __name__ == '__main__':
    # arg_parser = common.create_arg_parser()
    print("woo")
    # grpc_server, _ = common.services(arg_parser)
    # service_stub = common.create_service(grpc_server)
    # start_recording(service_stub)
