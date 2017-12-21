cmake_minimum_required(VERSION 2.8)

project(Classification_Algorithms_System)
file(GLOB SOURCES
    includes/*.hpp
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
