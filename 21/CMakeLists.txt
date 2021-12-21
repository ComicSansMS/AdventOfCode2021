add_library(21_dirac_dice STATIC dirac_dice.hpp dirac_dice.cpp)
target_include_directories(21_dirac_dice PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(21_dirac_dice PUBLIC fmt::fmt range-v3)
add_executable(advent21 advent21.cpp)
target_link_libraries(advent21 PUBLIC 21_dirac_dice)

file(COPY input DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(TARGET Catch)
    add_executable(21_dirac_dice_test dirac_dice.t.cpp)
    target_link_libraries(21_dirac_dice_test PUBLIC Catch 21_dirac_dice)
    add_test(NAME TestAdvent21 COMMAND 21_dirac_dice_test)
    enable_coverage(21_dirac_dice)
endif()
