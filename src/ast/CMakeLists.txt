add_library(ast INTERFACE)

target_include_directories(ast INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lexer PRIVATE core)
