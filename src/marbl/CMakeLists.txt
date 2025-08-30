add_library(marbl STATIC marbl.cpp marbl.hpp)

target_include_directories(marbl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(marbl PUBLIC core lexer ast parser)
