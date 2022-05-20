cmake_minimum_required(VERSION 3.20)
project(rbtree)

set(CMAKE_CXX_STANDARD 14)

add_executable(rbtree main.cpp RBtree.h pch.h RBtreeNode.h tool.h)

target_precompile_headers(rbtree PRIVATE pch.h)