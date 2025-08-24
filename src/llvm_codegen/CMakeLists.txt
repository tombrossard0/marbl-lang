add_library(llvm_codegen STATIC
    llvm_codegen.cpp
    llvm_codegen.hpp
)

target_include_directories(llvm_codegen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(llvm_codegen PUBLIC
    core lexer ast parser interpreter
    ${llvm_libs}
)
