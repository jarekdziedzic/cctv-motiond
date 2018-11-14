cmake_minimum_required(VERSION 3.10)
project(imgdiff)

set(CMAKE_CXX_STANDARD 17)

include_directories(imgdiff-src)

add_executable(imgdiff
        imgdiff-src/brightness.cpp
        imgdiff-src/brightness.h
        imgdiff-src/exposure.cpp
        imgdiff-src/exposure.h
        imgdiff-src/imgdiff.cpp
        imgdiff-src/imgdiff.h
        imgdiff-src/imgdiff_main.cpp)

target_link_libraries(imgdiff opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs stdc++fs)
