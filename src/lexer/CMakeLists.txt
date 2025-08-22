find_package(FLEX REQUIRED)

# Generate the lexer
flex_target(MarblLexer
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.hpp
)

# Build lexer library
add_library(lexer STATIC
    lexer.cpp
    ${FLEX_MarblLexer_OUTPUTS} # the generated lexer.yy.cpp
)

target_include_directories(lexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(lexer PRIVATE core)
