set(source_files
    grammar.cpp
    is_long_option_name.cpp
    is_short_option_name.cpp
    option.cpp
    parser.cpp)

foreach(source_file ${source_files})

    add_boost_unit_test(${source_file})

endforeach()

target_sources(grammar PRIVATE ${CMAKE_SOURCE_DIR}/src/options/grammar.cpp)

target_sources(parser
    PRIVATE ${CMAKE_SOURCE_DIR}/src/options/grammar.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/src/options/parser.cpp)
