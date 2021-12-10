add_library(10_syntax_scoring STATIC syntax_scoring.hpp syntax_scoring.cpp)
target_include_directories(10_syntax_scoring PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(10_syntax_scoring PUBLIC fmt::fmt range-v3)
add_executable(advent10 advent10.cpp)
target_link_libraries(advent10 PUBLIC 10_syntax_scoring)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(10_syntax_scoring_test syntax_scoring.t.cpp)
    target_link_libraries(10_syntax_scoring_test PUBLIC Catch 10_syntax_scoring)
    add_test(NAME TestAdvent10 COMMAND 10_syntax_scoring_test)
    enable_coverage(10_syntax_scoring)
endif()
