add_executable(msd_c_1d main_1d.cpp)
target_link_libraries(msd_c_1d PUBLIC ${PROJECT_NAME})
add_executable(msd_c_2d main_2d.cpp)
target_link_libraries(msd_c_2d PUBLIC ${PROJECT_NAME})