cmake_minimum_required(VERSION 3.30)
project(kalman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(submodules/external/xtl)
add_subdirectory(submodules/external/xtensor)

#add_subdirectory(submodules/external/matplotlib-cpp)
#include_directories(${PROJECT_SOURCE_DIR}/submodules/external/matplotlib-cpp)
#find_package(Python3 COMPONENTS Development NumPy REQUIRED)
#include_directories(${Python3_INCLUDE_DIRS})
#include_directories(${Python3_NumPy_INCLUDE_DIRS})

# Find Eigen3
find_package(Eigen3 REQUIRED)

# Add the subdirectories for xtl and xtensor
add_subdirectory(slam)
add_subdirectory(main)

