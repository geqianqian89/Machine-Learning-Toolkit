cmake_minimum_required(VERSION 3.6)
project(FeatureSelection_C___11)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES data.cpp sample.cpp embrapa.cpp utils.cpp kernel.cpp imap.cpp imadual.cpp perceptron.cpp adaboost.cpp adaboostdual.cpp almap.cpp smo.cpp golub.cpp fisher.cpp rfe.cpp main.cpp aos_hash.h aos_hash.cpp aos_heap.h aos_heap.cpp pl.cpp)
add_executable(FeatureSelection_C___11 ${SOURCE_FILES})