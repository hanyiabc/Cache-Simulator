cmake_minimum_required(VERSION 3.5)
project(Project3 CXX)

# require a C++11 compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simulate main.cpp SA_cache.h SA_cache.cpp)

