add_library(ParamHook STATIC
        # List your source files here.
        ParamHooks.c
        )

target_include_directories(
        ParamHook
        PRIVATE
        include
)

# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(
        ParamHook
        PRIVATE
        cxx_range_for cxx_auto_type
)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(
        ParamHook PROPERTIES
        COMPILE_FLAGS " -fPIC")