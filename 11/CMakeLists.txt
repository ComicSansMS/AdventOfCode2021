add_library(11_dumbo_octopus STATIC dumbo_octopus.hpp dumbo_octopus.cpp)
target_include_directories(11_dumbo_octopus PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(11_dumbo_octopus PUBLIC fmt::fmt range-v3)
add_executable(advent11 advent11.cpp)
target_link_libraries(advent11 PUBLIC 11_dumbo_octopus)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(11_dumbo_octopus_test dumbo_octopus.t.cpp)
    target_link_libraries(11_dumbo_octopus_test PUBLIC Catch 11_dumbo_octopus)
    add_test(NAME TestAdvent11 COMMAND 11_dumbo_octopus_test)
    enable_coverage(11_dumbo_octopus)
endif()
