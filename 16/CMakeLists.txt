add_library(16_packet_decoder STATIC packet_decoder.hpp packet_decoder.cpp)
target_include_directories(16_packet_decoder PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(16_packet_decoder PUBLIC fmt::fmt range-v3)
add_executable(advent16 advent16.cpp)
target_link_libraries(advent16 PUBLIC 16_packet_decoder)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(16_packet_decoder_test packet_decoder.t.cpp)
    target_link_libraries(16_packet_decoder_test PUBLIC Catch 16_packet_decoder)
    add_test(NAME TestAdvent16 COMMAND 16_packet_decoder_test)
    enable_coverage(16_packet_decoder)
endif()
