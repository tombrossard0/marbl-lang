add_library(core STATIC
    utils.hpp
    utils.cpp
    errors.hpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
