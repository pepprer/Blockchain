cmake_minimum_required(VERSION 3.17)
project(Blockchain)

set(CMAKE_CXX_STANDARD 14)

add_executable(Blockchain main.cpp header.h hFiles/Hash.h cppFiles/Hash.cpp)