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
