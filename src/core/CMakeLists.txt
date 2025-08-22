add_library(core STATIC
    utils.hpp
    utils.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
