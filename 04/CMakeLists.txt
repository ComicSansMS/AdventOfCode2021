add_library(04_giant_squid STATIC giant_squid.hpp giant_squid.cpp)
target_include_directories(04_giant_squid PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(04_giant_squid PUBLIC fmt::fmt range-v3)
add_executable(advent04 advent04.cpp)
target_link_libraries(advent04 PUBLIC 04_giant_squid)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(04_giant_squid_test giant_squid.t.cpp)
    target_link_libraries(04_giant_squid_test PUBLIC Catch 04_giant_squid)
    add_test(NAME TestAdvent04 COMMAND 04_giant_squid_test)
    enable_coverage(04_giant_squid)
endif()
