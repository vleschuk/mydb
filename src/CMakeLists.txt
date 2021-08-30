set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(test_storage
    storage.cpp
    error.cpp
    test_storage.cpp
)

add_executable(dbserver
    error.cpp
    connection.cpp
    connection_manager.cpp
    reply.cpp
    request_handler.cpp
    request_parser.cpp
    server.cpp
    storage.cpp
    main_server.cpp
)

add_executable(dbclient
    main_client.cpp
)
