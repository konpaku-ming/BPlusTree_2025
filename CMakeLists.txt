cmake_minimum_required(VERSION 3.29)
project(BPlusTree)

set(CMAKE_CXX_STANDARD 20)

add_executable(code main.cpp
        BPlusTree.cpp)
