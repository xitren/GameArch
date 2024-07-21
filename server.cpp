#include "unitcontrol.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <string>
#include <map>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using unitcontrol::RegisterRequest;
using unitcontrol::RegisterReply;
using unitcontrol::UnitPositionReply;
using unitcontrol::UnitSendReply;
using unitcontrol::MCCRegister;
using unitcontrol::UnitControl;

// Server Implementation
class MCCServiceImplementation final : public MCCRegister::Service {
private:
    std::string                  server_address = "0.0.0.0:50051";
    std::unique_ptr<Server>      server{};
    std::map<std::string, std::string> registry{};

    Status
    registerUnit(ServerContext* context, const RegisterRequest* request, RegisterReply* reply) override
    {
        // Obtains the original string from the request
        std::string a = request->name();
        std::cout << "Unit registered: " << a << std::endl;

        reply->set_session(a);
        return Status::OK;
    }

    Status
    unitPosition(ServerContext* context, const UnitPositionReply* request, UnitSendReply* reply) override
    {
        return Status::OK;
    }

public:
    MCCServiceImplementation() {
        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which
        // communication with client takes place
        builder.RegisterService(this);

        // Assembling the server
        server = builder.BuildAndStart();
        std::cout << "Server listening on port: " << server_address << std::endl;
    }

    void Wait() {
        server->Wait();
    }
};

int
main(int argc, char** argv)
{
    MCCServiceImplementation mng;
    mng.Wait();
    return 0;
}
