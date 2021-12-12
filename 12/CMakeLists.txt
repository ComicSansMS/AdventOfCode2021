add_library(12_passage_pathing STATIC passage_pathing.hpp passage_pathing.cpp)
target_include_directories(12_passage_pathing PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(12_passage_pathing PUBLIC fmt::fmt range-v3)
add_executable(advent12 advent12.cpp)
target_link_libraries(advent12 PUBLIC 12_passage_pathing)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(12_passage_pathing_test passage_pathing.t.cpp)
    target_link_libraries(12_passage_pathing_test PUBLIC Catch 12_passage_pathing)
    add_test(NAME TestAdvent12 COMMAND 12_passage_pathing_test)
    enable_coverage(12_passage_pathing)
endif()
