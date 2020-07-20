# The list of plugins
set(LLVM_PLUGINS
        FuncName
        )

set(FuncName_SOURCES
        FuncName.cpp
        )


foreach (plugin ${LLVM_PLUGINS})
    add_library(
            ${plugin}
            SHARED
            ${${plugin}_SOURCES}
    )
    target_include_directories(
            ${plugin}
            PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )
    target_link_libraries(
            ${plugin}
            demangle
    )
endforeach ()

