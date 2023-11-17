cmake_minimum_required(VERSION 3.5)

project(test_db LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_db main.cpp
    dbcontroller.h dbcontroller.cpp)

target_link_libraries(test_db pqxx)
