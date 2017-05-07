cmake_minimum_required(VERSION 3.6)
project(pv264_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Wold-style-cast -Werror -pedantic -Weffc++ ")

set(SOURCE_FILES
    catch.hpp
        FibHeap.hpp
    main.cpp)

add_executable(pv264_project ${SOURCE_FILES})