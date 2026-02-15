cmake_minimum_required(VERSION 3.10)


project(http-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

add_executable(http_server main.cpp HttpServer.cpp)
