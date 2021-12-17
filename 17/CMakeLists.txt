add_library(17_trick_shot STATIC trick_shot.hpp trick_shot.cpp)
target_include_directories(17_trick_shot PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(17_trick_shot PUBLIC fmt::fmt range-v3)
add_executable(advent17 advent17.cpp)
target_link_libraries(advent17 PUBLIC 17_trick_shot)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(17_trick_shot_test trick_shot.t.cpp)
    target_link_libraries(17_trick_shot_test PUBLIC Catch 17_trick_shot)
    add_test(NAME TestAdvent17 COMMAND 17_trick_shot_test)
    enable_coverage(17_trick_shot)
endif()
