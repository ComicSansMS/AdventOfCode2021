add_library(13_transparent_origami STATIC transparent_origami.hpp transparent_origami.cpp)
target_include_directories(13_transparent_origami PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(13_transparent_origami PUBLIC fmt::fmt range-v3)
add_executable(advent13 advent13.cpp)
target_link_libraries(advent13 PUBLIC 13_transparent_origami)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(13_transparent_origami_test transparent_origami.t.cpp)
    target_link_libraries(13_transparent_origami_test PUBLIC Catch 13_transparent_origami)
    add_test(NAME TestAdvent13 COMMAND 13_transparent_origami_test)
    enable_coverage(13_transparent_origami)
endif()
