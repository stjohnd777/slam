cmake_minimum_required(VERSION 3.30)
project(kalman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the subdirectories for xtl and xtensor
add_subdirectory(submodules/external/xtl)
add_subdirectory(submodules/external/xtensor)
#add_subdirectory(submodules/external/matplotlib-cpp)
include_directories(${PROJECT_SOURCE_DIR}/submodules/external/matplotlib-cpp)


find_package(Python3 COMPONENTS Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})


# Find Eigen3
find_package(Eigen3 REQUIRED)


add_executable(kalman main.cpp
        n_dims/probablity_distibution.h
        n_dims/world.h
        n_dims/robot.h
        n_dims/state.h)

# Link libraries
target_link_libraries(kalman PRIVATE
        Eigen3::Eigen
        xtensor
        Python3::Python
        ${Python3_LIBRARIES}

)
## If you have linking issues, you might need to add:
#if (APPLE)
#    set_target_properties(your_target_name PROPERTIES
#            LINK_FLAGS "-undefined dynamic_lookup"
#    )
#endif ()


# Include directories - adding the parent include directory
target_include_directories(kalman PRIVATE
        ${CMAKE_SOURCE_DIR}/submodules/external/xtl/include
        ${CMAKE_SOURCE_DIR}/submodules/external/xtensor/include
        ${EIGEN3_INCLUDE_DIRS})


# Include directories - adding the parent include directory
target_include_directories(kalman PRIVATE
        ${CMAKE_SOURCE_DIR}/submodules/external/xtl/include
        ${CMAKE_SOURCE_DIR}/submodules/external/xtensor/include
        ${EIGEN3_INCLUDE_DIRS}
)