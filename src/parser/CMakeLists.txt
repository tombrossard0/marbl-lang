add_library(parser STATIC
    parser_exception.hpp
    parser.hpp
    parser.cpp
)

target_include_directories(parser INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser PRIVATE core lexer ast)
