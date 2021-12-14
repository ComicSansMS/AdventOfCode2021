add_library(14_extended_polymerization STATIC extended_polymerization.hpp extended_polymerization.cpp)
target_include_directories(14_extended_polymerization PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(14_extended_polymerization PUBLIC fmt::fmt range-v3)
add_executable(advent14 advent14.cpp)
target_link_libraries(advent14 PUBLIC 14_extended_polymerization)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(14_extended_polymerization_test extended_polymerization.t.cpp)
    target_link_libraries(14_extended_polymerization_test PUBLIC Catch 14_extended_polymerization)
    add_test(NAME TestAdvent14 COMMAND 14_extended_polymerization_test)
    enable_coverage(14_extended_polymerization)
endif()
