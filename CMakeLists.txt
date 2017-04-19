cmake_minimum_required(VERSION 3.6)
project(pv264_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    catch.hpp
        FibHeap.hpp
    main.cpp
    test.cpp)

add_executable(pv264_project ${SOURCE_FILES})