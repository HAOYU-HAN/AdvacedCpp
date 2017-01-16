cmake_minimum_required(VERSION 3.4)
project(btree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp btree.h btree_iterator.h)
add_executable(btree ${SOURCE_FILES})