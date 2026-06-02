cmake_minimum_required(VERSION 3.23)

project(options
    LANGUAGES CXX
    VERSION   1.0.0)

option(enable_unit_tests "enable unit tests" OFF)

if (${enable_unit_tests})

    enable_testing()

    add_subdirectory(test)

endif()

add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED    ON
    LIBRARY_OUTPUT_DIRECTORY lib
    SOVERSION                ${PROJECT_VERSION_MAJOR}
    VERSION                  ${PROJECT_VERSION})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_compile_options(${PROJECT_NAME}
    PRIVATE "$<$<CXX_COMPILER_ID:GNU>:--debug;-Wall;-Werror;-Wextra>")

target_sources(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/src/options/grammar.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/options/parser.cpp

    PUBLIC
        FILE_SET  HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include)
