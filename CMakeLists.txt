cmake_minimum_required(VERSION 3.20)
project(scheme_implementation)

set(CMAKE_CXX_STANDARD 14)

add_executable(scheme_implementation
        main.cpp
        packet.cpp
        router.cpp
        packet_queue.cpp
        infrastructure.cpp
        load_balancer.cpp
        backend_server.cpp
        middle_box.cpp
        client.cpp
        config.cpp)
