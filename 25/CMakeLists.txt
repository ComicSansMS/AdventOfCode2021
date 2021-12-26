add_library(25_sea_cucumber STATIC sea_cucumber.hpp sea_cucumber.cpp)
target_include_directories(25_sea_cucumber PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(25_sea_cucumber PUBLIC fmt::fmt range-v3)
add_executable(advent25 advent25.cpp)
target_link_libraries(advent25 PUBLIC 25_sea_cucumber)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(25_sea_cucumber_test sea_cucumber.t.cpp)
    target_link_libraries(25_sea_cucumber_test PUBLIC Catch 25_sea_cucumber)
    add_test(NAME TestAdvent25 COMMAND 25_sea_cucumber_test)
    enable_coverage(25_sea_cucumber)
endif()
