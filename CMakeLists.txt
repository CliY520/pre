cmake_minimum_required(VERSION 2.6)
project(an)
SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_executable(ancpp an.cpp fileRead.cpp)
# target_sources(ancpp )
