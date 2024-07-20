from __future__ import print_function

import logging

import grpc
import stringreverse_pb2
import stringreverse_pb2_grpc


def run():
    # NOTE(gRPC Python Team): .close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    print("Will try to reverse word ...")
    with grpc.insecure_channel('172.16.128.2:50051') as channel:
        stub = stringreverse_pb2_grpc.StringReverseStub(channel)
        response = stub.sendRequest(stringreverse_pb2.StringRequest(original='grpc is cool!'))
    print("Reverse client received: " + response.reversed)


if __name__ == '__main__':
    logging.basicConfig()
    run()
