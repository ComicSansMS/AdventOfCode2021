add_library(18_snailfish STATIC snailfish.hpp snailfish.cpp)
target_include_directories(18_snailfish PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(18_snailfish PUBLIC fmt::fmt range-v3)
add_executable(advent18 advent18.cpp)
target_link_libraries(advent18 PUBLIC 18_snailfish)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(18_snailfish_test snailfish.t.cpp)
    target_link_libraries(18_snailfish_test PUBLIC Catch 18_snailfish)
    add_test(NAME TestAdvent18 COMMAND 18_snailfish_test)
    enable_coverage(18_snailfish)
endif()
