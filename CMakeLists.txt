cmake_minimum_required(VERSION 3.26)
project(X13)

include_directories(mir/include/miral)

#include_directories(/usr/include/mirplatform) # that's the path in public headers
include_directories(mir/include/mir/platform)

#include_directories(/usr/include/mircore) # that's the path in public headers
include_directories(mir/include/core)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wno-unused-parameter")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(X13 main.cpp)
target_link_libraries(X13 miral mirplatform mircore)
