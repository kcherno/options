cmake_minimum_required(VERSION 3.11)

project(options
    VERSION   1.0.0
    LANGUAGES CXX)

add_compile_options("$<$<CXX_COMPILER_ID:GNU>:--debug;-Wall;-Werror;-Wextra>")

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(enable_unit_tests "enable unit tests" OFF)

if (${enable_unit_tests})

    enable_testing()

    add_subdirectory(test)

endif()

add_library(${CMAKE_PROJECT_NAME} SHARED)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY lib
    VERSION                  ${CMAKE_PROJECT_VERSION}
    SOVERSION                ${CMAKE_PROJECT_VERSION_MAJOR})

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/options/grammar.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/options/parser.cpp)
