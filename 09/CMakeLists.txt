add_library(09_smoke_basin STATIC smoke_basin.hpp smoke_basin.cpp)
target_include_directories(09_smoke_basin PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(09_smoke_basin PUBLIC fmt::fmt range-v3)
add_executable(advent09 advent09.cpp)
target_link_libraries(advent09 PUBLIC 09_smoke_basin)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(09_smoke_basin_test smoke_basin.t.cpp)
    target_link_libraries(09_smoke_basin_test PUBLIC Catch 09_smoke_basin)
    add_test(NAME TestAdvent09 COMMAND 09_smoke_basin_test)
    enable_coverage(09_smoke_basin)
endif()
