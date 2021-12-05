add_library(05_hydrothermal_venture STATIC hydrothermal_venture.hpp hydrothermal_venture.cpp)
target_include_directories(05_hydrothermal_venture PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(05_hydrothermal_venture PUBLIC fmt::fmt range-v3)
add_executable(advent05 advent05.cpp)
target_link_libraries(advent05 PUBLIC 05_hydrothermal_venture)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(05_hydrothermal_venture_test hydrothermal_venture.t.cpp)
    target_link_libraries(05_hydrothermal_venture_test PUBLIC Catch 05_hydrothermal_venture)
    add_test(NAME TestAdvent05 COMMAND 05_hydrothermal_venture_test)
    enable_coverage(05_hydrothermal_venture)
endif()
