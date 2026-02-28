cmake_minimum_required(VERSION 3.2)
project(final_project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(final_project
        client.cpp
        InventoryManager.cpp
        InventoryManager.h
        Item.cpp
        Item.h
        server.cpp)
