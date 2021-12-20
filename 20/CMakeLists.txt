add_library(20_trench_map STATIC trench_map.hpp trench_map.cpp)
target_include_directories(20_trench_map PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(20_trench_map PUBLIC fmt::fmt range-v3)
add_executable(advent20 advent20.cpp)
target_link_libraries(advent20 PUBLIC 20_trench_map)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(20_trench_map_test trench_map.t.cpp)
    target_link_libraries(20_trench_map_test PUBLIC Catch 20_trench_map)
    add_test(NAME TestAdvent20 COMMAND 20_trench_map_test)
    enable_coverage(20_trench_map)
endif()
