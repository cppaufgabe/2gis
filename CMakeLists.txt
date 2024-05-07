cmake_minimum_required(VERSION 3.20)
project(rl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test 2gis.cpp inputparcer.cpp func.cpp)
target_compile_options(test PRIVATE -Ofast) #tail recursion

# Link runTests with what we want to test and the GTest and pthread library
add_executable(unittests gtest/tests.cpp)
target_link_libraries(unittests GTest::gtest_main)
