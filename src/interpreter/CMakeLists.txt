add_library(interpreter STATIC
    interpreter.cpp
    interpreter.hpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(interpreter PUBLIC core lexer ast parser)
