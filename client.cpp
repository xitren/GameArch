
#include "unitcontrol.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

using unitcontrol::RegisterRequest;
using unitcontrol::RegisterReply;
using unitcontrol::UnitPositionReply;
using unitcontrol::UnitPositionReply_StateType;
using unitcontrol::UnitPositionRequest;
using unitcontrol::UnitSendReply;
using unitcontrol::UnitSendRequest;
using unitcontrol::MCCRegister;
using unitcontrol::UnitControl;

class UnitControlImplementation final : public UnitControl::Service {
private:
    std::string                  server_address = "0.0.0.0:50052";
    std::unique_ptr<Server>      server{};
    UnitPositionReply_StateType  state{UnitPositionReply_StateType::UnitPositionReply_StateType_idle};

    const struct TechParameters {
        float dx{0.05};
        float dy{0.05};
    } params{};

    struct PositionState {
        float x{};
        float y{};
        float vx{};
        float vy{};
        float ax{};
        float ay{};
    } position{};

    struct PositionTarget {
        float targ_x{};
        float targ_y{};
        float targ_vx{};
        float targ_vy{};
        float epsilon{};
    } target{};

    Status
    getPosition(ServerContext* context, const UnitPositionRequest* request, UnitPositionReply* reply) override
    {
        reply->set_id(request->id());
        reply->set_session(request->session());
        reply->set_name(request->name());
        reply->set_state(state);
        reply->set_x(position.x);
        reply->set_y(position.y);
        reply->set_vx(position.vx);
        reply->set_vy(position.vy);
        reply->set_ax(position.ax);
        reply->set_ay(position.ay);
        reply->set_x(position.x);
        return Status::OK;
    }

    Status
    sendToPosition(ServerContext* context, const UnitSendRequest* request, UnitSendReply* reply) override
    {
        target.targ_x = request->x();
        target.targ_y = request->y();
        target.targ_vx = request->vx();
        target.targ_vy = request->vy();
        target.epsilon = request->epsilon();
        reply->set_id(request->id());
        reply->set_session(request->session());
        // reply->set_state(state);
        return Status::OK;
    }

public:
    UnitControlImplementation() {
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

class MCCClient {
public:
    MCCClient(std::shared_ptr<Channel> channel) : stub_(MCCRegister::NewStub(channel))
    {}

    // Assembles client payload, sends it to the server, and returns its response
    std::string
    registerUnit(std::string name)
    {
        // Data to be sent to server
        RegisterRequest request;
        request.set_name(name);

        // Container for server response
        RegisterReply reply;

        // Context can be used to send meta data to server or modify RPC behaviour
        ClientContext context;

        // Actual Remote Procedure Call
        Status status = stub_->registerUnit(&context, request, &reply);

        // Returns results based on RPC status
        if (status.ok()) {
            return reply.session();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "RPC Failed";
        }
    }

private:
    std::unique_ptr<MCCRegister::Stub> stub_;
};

void
RunClient()
{
    std::string target_address("localhost:50051");
    // Instantiates the client
    MCCClient client(
        // Channel from which RPCs are made - endpoint is the target_address
        grpc::CreateChannel(target_address,
                            // Indicate when channel is not authenticated
                            grpc::InsecureChannelCredentials()));

    std::string response;
    std::string a = "grpc is cool!";

    // Wait for server 5 sec

    // RPC is created and response is stored
    response = client.registerUnit(a);

    // Prints results
    std::cout << "Original string: " << a << std::endl;
    std::cout << "Reversed string: " << response << std::endl;
}

int
main(int argc, char* argv[])
{
    RunClient();

    return 0;
}
