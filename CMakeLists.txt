cmake_minimum_required(VERSION 3.20)
project(rb_tree_test C CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-O2")

add_executable(rb_tree_test main.cpp)

target_include_directories(rb_tree_test PRIVATE src)

target_precompile_headers(rb_tree_test PRIVATE pch.h)