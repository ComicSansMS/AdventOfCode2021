add_library(22_reactor_reboot STATIC reactor_reboot.hpp reactor_reboot.cpp)
target_include_directories(22_reactor_reboot PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(22_reactor_reboot PUBLIC fmt::fmt range-v3)
add_executable(advent22 advent22.cpp)
target_link_libraries(advent22 PUBLIC 22_reactor_reboot)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(22_reactor_reboot_test reactor_reboot.t.cpp)
    target_link_libraries(22_reactor_reboot_test PUBLIC Catch 22_reactor_reboot)
    add_test(NAME TestAdvent22 COMMAND 22_reactor_reboot_test)
    enable_coverage(22_reactor_reboot)
endif()
