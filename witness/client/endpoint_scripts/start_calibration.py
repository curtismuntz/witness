#!/usr/bin/env python3

import common
import witness.api.witness_pb2 as witness_pb2
import witness.api.calibration_pb2 as calibration_pb2
import witness.api.witness_pb2_grpc as witness_pb2_grpc

DEFAULT_TIMEOUT = 5  # seconds


def start_calibration(service_stub, fname=""):
    calib_request = calibration_pb2.StartCalibrationRequest()
    calib_request.chessboard.board_size_width = 8
    calib_request.chessboard.board_size_height = 6
    calib_request.chessboard.square_size = 50
    calib_request.chessboard.window_size = 1
    calib_request.chessboard.minimum_calibration_samples = 50
    calib_reply = service_stub.StartCalibration(calib_request, 50)
    for reply in calib_reply:
        print(calib_reply)


if __name__ == '__main__':
    arg_parser = common.create_arg_parser()
    grpc_server, _ = common.services(arg_parser)
    service_stub = common.create_service(grpc_server)
    start_calibration(service_stub)
