add_library(
        demangle
        SHARED
        demangle.cpp
)

target_include_directories(
        demangle
        PRIVATE
        ${RUSTC_DEMANGLE_INCLUDE_DIR}
)

target_link_libraries(
        demangle
        ${RUSTC_DEMANGLE}
)