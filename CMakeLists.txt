cmake_minimum_required(VERSION 3.12)

project(glem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(glem)
add_subdirectory(sandbox)
