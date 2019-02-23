#!/usr/bin/env python3

import argparse
import os
import grpc
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc


def create_arg_parser():
    arg_parser = argparse.ArgumentParser(description="GUI for interfacing with the onboard server")
    arg_parser.add_argument(
        "--device_server",
        default=os.environ.get("WITNESS_SERVER", "localhost:50051"),
        help="The address of the witness API server")
    return arg_parser


def services(arg_parser):
    args = arg_parser.parse_args()
    grpc_server = args.device_server + ":50051"
    file_server = args.device_server + ":8000"
    return grpc_server, file_server


def create_service(grpc_server):
    channel = grpc.insecure_channel(grpc_server)
    service_stub = witness_pb2_grpc.WitnessCameraServiceStub(channel)
    return service_stub
