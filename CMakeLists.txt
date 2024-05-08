cmake_minimum_required(VERSION 3.21)

project(TESTS_MMAP LANGUAGES CXX)

add_executable(tests_mmap main.cpp)

set_target_properties(tests_mmap PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE
        CXX_EXTENSIONS FALSE)
