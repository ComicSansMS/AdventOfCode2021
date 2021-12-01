add_library(01_sonar_sweep STATIC sonar_sweep.hpp sonar_sweep.cpp)
target_include_directories(01_sonar_sweep PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(01_sonar_sweep PUBLIC fmt::fmt range-v3)
add_executable(advent01 advent01.cpp)
target_link_libraries(advent01 PUBLIC 01_sonar_sweep)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(01_sonar_sweep_test sonar_sweep.t.cpp)
    target_link_libraries(01_sonar_sweep_test PUBLIC Catch 01_sonar_sweep)
    add_test(NAME TestAdvent01 COMMAND 01_sonar_sweep_test)
    enable_coverage(01_sonar_sweep)
endif()
