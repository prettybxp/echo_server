cmake_minimum_required(VERSION 2.6)

#include(FindProtobuf)
#include(FindPkgConfig)
#pkg_check_modules(PROTOBUF REQUIRED protobuf)

#message("PROTOBUF_LIBRARIES=${PROTOBUF_LIBRARIES}")
#message("PROTOBUF_LIBRARIES=${PROTOBUF_LIBRARY_DIRS}")
#message("PROTOBUF_LIBRARIES=${PROTOBUF_INCLUDE_DIRS}")

find_package(Boost 1.64.0 COMPONENTS system coroutine)
message("Boost_LIBRARIES=${Boost_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


include_directories(${CMAKE_SOURCE_DIR}/../prefix_root_local/include)

set(SRC Main.cpp)

add_executable(test ${SRC})


#target_link_libraries(test pthread)

target_link_libraries(test ${Boost_LIBRARIES})