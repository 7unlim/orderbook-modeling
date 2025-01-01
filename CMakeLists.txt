# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name
project(OrderBookProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(includes)

# Add executable for the main file
add_executable(main
    src/main.cpp
    src/OrderBook.cpp
)

# Specify target link libraries if needed (none specified here)
