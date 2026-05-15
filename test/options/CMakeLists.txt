set(source_files grammar.cpp option.cpp)

foreach(source_file ${source_files})

    add_boost_unit_test(${source_file})

endforeach()

target_sources(grammar PRIVATE ${CMAKE_SOURCE_DIR}/src/options/grammar.cpp)
