add_library(19_beacon_scanner STATIC beacon_scanner.hpp beacon_scanner.cpp)
target_include_directories(19_beacon_scanner PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(19_beacon_scanner PUBLIC fmt::fmt range-v3)
add_executable(advent19 advent19.cpp)
target_link_libraries(advent19 PUBLIC 19_beacon_scanner)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(19_beacon_scanner_test beacon_scanner.t.cpp)
    target_link_libraries(19_beacon_scanner_test PUBLIC Catch 19_beacon_scanner)
    add_test(NAME TestAdvent19 COMMAND 19_beacon_scanner_test)
    enable_coverage(19_beacon_scanner)
endif()
