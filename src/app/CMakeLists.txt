add_executable(marbl main.cpp)

target_link_libraries(marbl
    PRIVATE
        lexer
)

# Put the executable directly in the build/ folder
set_target_properties(marbl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
