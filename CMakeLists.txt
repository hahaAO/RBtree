cmake_minimum_required(VERSION 3.20)
project(rbtree)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-O2")

add_executable(rbtree main.cpp RBtree.h pch.h RBtreeNode.h tool.h RBtree_implementation.h RBtreeNode_implementation.h)

target_precompile_headers(rbtree PRIVATE pch.h)