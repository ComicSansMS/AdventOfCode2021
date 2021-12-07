add_library(07_treachery_of_whales STATIC treachery_of_whales.hpp treachery_of_whales.cpp)
target_include_directories(07_treachery_of_whales PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(07_treachery_of_whales PUBLIC fmt::fmt range-v3)
add_executable(advent07 advent07.cpp)
target_link_libraries(advent07 PUBLIC 07_treachery_of_whales)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(07_treachery_of_whales_test treachery_of_whales.t.cpp)
    target_link_libraries(07_treachery_of_whales_test PUBLIC Catch 07_treachery_of_whales)
    add_test(NAME TestAdvent07 COMMAND 07_treachery_of_whales_test)
    enable_coverage(07_treachery_of_whales)
endif()
