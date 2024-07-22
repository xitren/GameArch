cmake_minimum_required(VERSION 3.21)

project(stringreverse
        DESCRIPTION "qRPC example")

# Protobuf
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "protos/unitcontrol.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/unitcontrol.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/unitcontrol.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/unitcontrol.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/unitcontrol.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include(FetchContent)
FetchContent_Declare(
		patterns
		GIT_REPOSITORY https://github.com/xitren/patterns.git
		GIT_TAG        v1.0.1
)
FetchContent_MakeAvailable(patterns)

foreach(_target
        client server)
        add_executable(${_target} "${_target}.cpp"
                ${hw_proto_srcs}
                ${hw_grpc_srcs})

        target_link_libraries(${_target}
                ${_REFLECTION}
                ${_GRPC_GRPCPP}
                ${_PROTOBUF_LIBPROTOBUF}
                patterns_lib)
        target_compile_features(${_target} PUBLIC cxx_std_20)
        target_compile_options(${_target}
                PRIVATE
                -Wall -Wextra -Wpedantic -Weffc++ -std=c++20 -Wc++20-compat)
        target_link_options(${_target}
                PRIVATE
                -Wl,-demangle)
endforeach()
