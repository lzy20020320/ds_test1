cmake_minimum_required(VERSION 3.19)
project(adj_list_dir_network)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(adj_list_dir_network
        AdjListDirNetwork.h
        Assistance.h
        TestAdjListDirNetwork.cpp)
