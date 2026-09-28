cmake_minimum_required(VERSION 3.20)

project(tinygl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(EXPORT_COMPILE_COMMANDS ON)

add_library(tinygl
        src/tinygl.cpp
        third_party/glad/glad.c
)

option(TINYGL_BUILD_TESTS "Build TinyGL tests" ON)

if(TINYGL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_include_directories(tinygl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR/include}

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad
)