add_library(03_binary_diagnostic STATIC binary_diagnostic.hpp binary_diagnostic.cpp)
target_include_directories(03_binary_diagnostic PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(03_binary_diagnostic PUBLIC fmt::fmt range-v3)
add_executable(advent03 advent03.cpp)
target_link_libraries(advent03 PUBLIC 03_binary_diagnostic)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(03_binary_diagnostic_test binary_diagnostic.t.cpp)
    target_link_libraries(03_binary_diagnostic_test PUBLIC Catch 03_binary_diagnostic)
    add_test(NAME TestAdvent03 COMMAND 03_binary_diagnostic_test)
    enable_coverage(03_binary_diagnostic)
endif()
