cmake_minimum_required(VERSION 3.16)
project(armor_project)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENCV REQUIRED opencv4)

include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})


add_executable(task1 task1.cpp)
target_link_libraries(task1 ${OPENCV_LIBRARIES})


add_executable(task2 task2.cpp)
target_link_libraries(task2 ${OPENCV_LIBRARIES})

