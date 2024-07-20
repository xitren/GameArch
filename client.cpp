
#include "unitcontrol.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using unitcontrol::RegisterRequest;
using unitcontrol::RegisterReply;
using unitcontrol::UnitPositionReply;
using unitcontrol::UnitSendReply;
using unitcontrol::MCCRegister;

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
