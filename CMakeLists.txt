cmake_minimum_required(VERSION 3.25)
project(MARBL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_DIR}")

llvm_map_components_to_libnames(llvm_libs core orcjit native)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(src/core)
add_subdirectory(src/lexer)
add_subdirectory(src/ast)
add_subdirectory(src/parser)
add_subdirectory(src/interpreter)
add_subdirectory(src/llvm_codegen)
add_subdirectory(src/app)
add_subdirectory(src/marbl)

add_subdirectory(tests)
add_subdirectory(examples)
