cmake_minimum_required(VERSION 2.8)

project(UFJF-MLTK)
file(GLOB SOURCES
    includes/*.hpp
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} includes/AOS.hpp src/AOS.cpp src/Hash.cpp includes/Hash.hpp src/Heap.cpp includes/Heap.hpp)