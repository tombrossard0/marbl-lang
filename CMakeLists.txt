cmake_minimum_required(VERSION 3.25)
project(MARBL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/core)
add_subdirectory(src/lexer)
add_subdirectory(src/ast)
add_subdirectory(src/parser)
add_subdirectory(src/app)
add_subdirectory(src/marbl)

add_subdirectory(tests)
add_subdirectory(examples)
