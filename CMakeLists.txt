cmake_minimum_required(VERSION 3.20)

project(tinygl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dist/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dist/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/dist/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dist/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dist/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/dist/lib)

option(TINYGL_BUILD_SHARED "Build TinyGL as a shared library" ON)
option(TINYGL_BUILD_STATIC "Build TinyGL as a static library" ON)

find_package(glfw3 CONFIG REQUIRED)

set(TINYGL_SOURCES
    src/tinygl.cpp
    include/glad/glad.c
)

function(tinygl_configure target)
    target_include_directories(${target}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    target_link_libraries(${target} PUBLIC glfw ${CMAKE_DL_LIBS})

    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8)
    endif()
endfunction()

if(TINYGL_BUILD_SHARED)
    add_library(tinygl SHARED ${TINYGL_SOURCES})
    add_library(tinygl::tinygl ALIAS tinygl)

    tinygl_configure(tinygl)

    target_compile_definitions(tinygl
        PUBLIC TINYGL_SHARED GLAD_GLAPI_EXPORT
        PRIVATE TINYGL_BUILD GLAD_GLAPI_EXPORT_BUILD
    )

    set_target_properties(tinygl PROPERTIES
        OUTPUT_NAME tinygl
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )

    install(TARGETS tinygl
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

if(TINYGL_BUILD_STATIC)
    add_library(tinygl_static STATIC ${TINYGL_SOURCES})
    add_library(tinygl::tinygl_static ALIAS tinygl_static)

    tinygl_configure(tinygl_static)

    if(WIN32)
        set_target_properties(tinygl_static PROPERTIES OUTPUT_NAME tinygl_static)
    else()
        set_target_properties(tinygl_static PROPERTIES OUTPUT_NAME tinygl)
    endif()

    set_target_properties(tinygl_static PROPERTIES POSITION_INDEPENDENT_CODE ON)

    install(TARGETS tinygl_static
        ARCHIVE DESTINATION lib
    )
endif()

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h")