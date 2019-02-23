#!/usr/bin/env python3

import common
import os
import urllib.request
import witness.api.witness_pb2 as witness_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc
import list_files

DEFAULT_TIMEOUT = 5  # seconds


def download_all(service_stub, file_server, destination_dir=os.path.expanduser("~/Downloads")):
    files = list_files.list_files(service_stub, quiet=True)
    for f in files:
        fname = os.path.basename(f)
        url = "http://" + file_server + "/" + fname
        print("Downloading %s" % url)
        urllib.request.urlretrieve(url, destination_dir + "/" + fname)


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, file_server = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    download_all(service_stub, file_server, destination_dir=os.path.expanduser("~/Downloads"))
