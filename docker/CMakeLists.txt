cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME sal_example)

project(${TARGET_NAME} DESCRIPTION "SAL server and client prototypes")

add_subdirectory(sal_server_prototype)
add_subdirectory(sal_client_prototype)
