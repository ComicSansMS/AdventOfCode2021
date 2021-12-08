add_library(08_seven_segment_search STATIC seven_segment_search.hpp seven_segment_search.cpp)
target_include_directories(08_seven_segment_search PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(08_seven_segment_search PUBLIC fmt::fmt range-v3)
add_executable(advent08 advent08.cpp)
target_link_libraries(advent08 PUBLIC 08_seven_segment_search)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(08_seven_segment_search_test seven_segment_search.t.cpp)
    target_link_libraries(08_seven_segment_search_test PUBLIC Catch 08_seven_segment_search)
    add_test(NAME TestAdvent08 COMMAND 08_seven_segment_search_test)
    enable_coverage(08_seven_segment_search)
endif()
