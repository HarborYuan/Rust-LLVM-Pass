cmake_minimum_required(VERSION 3.10)
project(RustGFPass)

# Set CXX Standard
set(CMAKE_CXX_STANDARD 11)

# find LLVM
find_package(LLVM 9.0.0 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LT_LLVM_INSTALL_DIR}")
message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
        )


# Set the LLVM header and library paths
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})


# Set local include directories
include_directories("${PROJECT_SOURCE_DIR}/include/")



# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()


# find utils
find_library(RUSTC_DEMANGLE
        REQUIRED
        NAMES "rustc_demangle" # Only for Linux now
        HINTS "${CMAKE_CURRENT_SOURCE_DIR}/utils/rustc-demangle/target/release/"
        NO_DEFAULT_PATH
        )

find_path(RUSTC_DEMANGLE_INCLUDE_DIR
        NAMES rustc_demangle.h
        HINTS "${CMAKE_CURRENT_SOURCE_DIR}/utils/rustc-demangle/crates/capi/include"
        NO_DEFAULT_PATH
        )
MESSAGE("demangle found : ${RUSTC_DEMANGLE}")
MESSAGE("demangle include dir found : ${RUSTC_DEMANGLE_INCLUDE_DIR}")



add_subdirectory(lib)
add_subdirectory(runtime)
add_subdirectory(utils)
add_subdirectory(test)