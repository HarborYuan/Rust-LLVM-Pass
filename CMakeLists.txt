cmake_minimum_required(VERSION 3.1)
project(RustGFPass)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -fno-exceptions -fno-rtti")
find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
message($ENV{PATH})

link_directories(${LLVM_LIBRARY_DIRS})
include_directories("${PROJECT_SOURCE_DIR}/include/")



add_subdirectory(lib)
add_subdirectory(runtime)
add_subdirectory(utils)