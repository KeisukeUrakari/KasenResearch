cmake_minimum_required(VERSION 3.10)

set(Project rpmtest)
set(Ver 0.1)

project(${Project} CXX C)

add_library(${Project} SHARED rpm-test.cpp)
target_compile_options(${Project} PUBLIC -g -Wall)
set_target_properties(${Project} PROPERTIES SUFFIX .${Ver}.so)