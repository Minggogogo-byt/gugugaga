cmake_minimum_required(VERSION 3.16)
project(camera_calib)
set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(calib main.cpp)
target_link_libraries(calib ${OpenCV_LIBS})
enable_testing()
add_test(
    NAME calib_test
    COMMAND calib
)
