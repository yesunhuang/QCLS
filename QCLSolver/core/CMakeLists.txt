cmake_minimum_required(VERSION 3.10)
project(test)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test 
    cluster_expansion.cpp 
    common.cpp 
    complex.cpp 
    deb.cpp 
    derive_data.cpp 
    node.cpp 
    operator_tree_iterator.cpp 
    operator_tree.cpp 
    static_dtree.cpp 
    test.cpp    
)
