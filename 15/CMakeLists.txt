add_library(15_chiton STATIC chiton.hpp chiton.cpp)
target_include_directories(15_chiton PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(15_chiton PUBLIC fmt::fmt range-v3)
add_executable(advent15 advent15.cpp)
target_link_libraries(advent15 PUBLIC 15_chiton)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(15_chiton_test chiton.t.cpp)
    target_link_libraries(15_chiton_test PUBLIC Catch 15_chiton)
    add_test(NAME TestAdvent15 COMMAND 15_chiton_test)
    enable_coverage(15_chiton)
endif()
