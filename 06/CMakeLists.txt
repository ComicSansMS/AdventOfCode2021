add_library(06_lanternfish STATIC lanternfish.hpp lanternfish.cpp)
target_include_directories(06_lanternfish PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(06_lanternfish PUBLIC fmt::fmt range-v3)
add_executable(advent06 advent06.cpp)
target_link_libraries(advent06 PUBLIC 06_lanternfish)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(06_lanternfish_test lanternfish.t.cpp)
    target_link_libraries(06_lanternfish_test PUBLIC Catch 06_lanternfish)
    add_test(NAME TestAdvent06 COMMAND 06_lanternfish_test)
    enable_coverage(06_lanternfish)
endif()
