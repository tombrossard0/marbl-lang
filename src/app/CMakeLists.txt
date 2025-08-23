add_executable(marbl_app main.cpp)

target_link_libraries(marbl_app
    PRIVATE
        marbl
        ast
)

# Put the executable directly in the build/ folder
set_target_properties(marbl_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
