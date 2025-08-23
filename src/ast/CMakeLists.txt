add_library(ast STATIC
    ast.hpp
    printer.hpp
    printer.cpp
)

target_include_directories(ast INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ast PRIVATE core)
