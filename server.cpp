#include "unitcontrol.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <string>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using unitcontrol::RegisterRequest;
using unitcontrol::RegisterReply;
using unitcontrol::UnitPositionReply;
using unitcontrol::UnitSendReply;
using unitcontrol::MCCRegister;

// Server Implementation
class MCCServiceImplementation final : public MCCRegister::Service {
    Status
    registerUnit(ServerContext* context, const RegisterRequest* request, RegisterReply* reply) override
    {
        // Obtains the original string from the request
        std::string a = request->name();
        std::cout << "Unit registered: " << a << std::endl;

        reply->set_session(a);
        return Status::OK;
    }
};

void
RunServer()
{
    std::string                  server_address("0.0.0.0:50051");
    MCCServiceImplementation     service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which
    // communication with client takes place
    builder.RegisterService(&service);

    // Assembling the server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << server_address << std::endl;

    server->Wait();
}

int
main(int argc, char** argv)
{
    RunServer();
    return 0;
}
