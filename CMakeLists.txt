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
        cpp/src/layers/interfaces/layer_pipe.cpp
        cpp/src/layers/interfaces/layer_tcp.cpp
        cpp/src/layers/interfaces/layer_ws.cpp

        cpp/src/layers/session/layer_session.cpp

        cpp/src/layers/protocols/protocol_json.cpp

        cpp/src/broker.cpp
        cpp/src/broker/broker_cmd.cpp
        cpp/src/broker/broker_layers.cpp
        cpp/src/broker/broker_repositories.cpp
        cpp/src/broker/broker_tags.cpp

        cpp/src/repository/broker_repository.cpp
        cpp/src/repository/broker_repository_db.cpp
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
        cpp/test/broker.cpp
        cpp/test/layers.cpp
        cpp/test/tags.cpp
)
