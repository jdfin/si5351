cmake_minimum_required(VERSION 3.12.0)
project(si5351 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_PARALLEL_LEVEL 1)

add_compile_options(-Wall)

add_executable(si5351_test
    si5351.cpp
    i2c_dev.cpp
    si5351_test.cpp)
