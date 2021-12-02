add_library(02_dive STATIC dive.hpp dive.cpp)
target_include_directories(02_dive PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(02_dive PUBLIC fmt::fmt range-v3)
add_executable(advent02 advent02.cpp)
target_link_libraries(advent02 PUBLIC 02_dive)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(02_dive_test dive.t.cpp)
    target_link_libraries(02_dive_test PUBLIC Catch 02_dive)
    add_test(NAME TestAdvent02 COMMAND 02_dive_test)
    enable_coverage(02_dive)
endif()
