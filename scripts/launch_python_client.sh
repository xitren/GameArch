#!/bin/bash

SERVICE="stringreverse"

cd ../
echo "Generate gRPC ${SERVICE} for python client"
python3 -m grpc_tools.protoc -Iprotos --python_out=. --pyi_out=. --grpc_python_out=. protos/${SERVICE}.proto

echo "Launching interactive ${SERVICE} python client"
python3 client.py
