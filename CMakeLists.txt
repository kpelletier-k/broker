cmake_minimum_required(VERSION 3.20)

if(NOT TARGET cmake-tools)
    include(FetchContent)
    FetchContent_Declare(cmake-tools
            GIT_REPOSITORY 	https://github.com/LithologIQ/cmake-tools.git
            GIT_TAG        	main
            SOURCE_DIR		${CMAKE_SOURCE_DIR}/external/cmake-tools)
    FetchContent_MakeAvailable(cmake-tools)
endif()

project(broker CXX)

create_static_lib(
        cpp/src/layers/layer_pipe.cpp
        cpp/src/layers/layer_tcp.cpp
        cpp/src/layers/layer_ws.cpp

        cpp/src/protocols/protocol_json.cpp

        cpp/src/broker.cpp
        cpp/src/broker_repository.cpp
        cpp/src/broker_repository_db.cpp
        cpp/src/broker_repository_tag.cpp
)

depends(console
        events
        thread_pool
        GIT_REPOSITORY https://github.com/LithologIQ/cpp_tools.git
        GIT_TAG main)

depends(GIT_REPOSITORY https://github.com/lithologiq/tag.git
        GIT_TAG main)

depends(GIT_REPOSITORY https://github.com/LithologIQ/json_rpc.git
        GIT_TAG main)

depends(GIT_REPOSITORY https://github.com/LithologIQ/sqlite.git
        GIT_TAG main)

depends(socket_tcp
        GIT_REPOSITORY https://github.com/LithologIQ/peripheral_io.git
        GIT_TAG main)

create_cpp_test(
        cpp/test/main.cpp
        cpp/test/layers.cpp
        cpp/test/tags.cpp
)
