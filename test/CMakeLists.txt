add_executable(demangle_test demangle.cpp)
target_include_directories(demangle_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(demangle_test demangle)
