cmake_minimum_required(VERSION 3.30)
project(lib_slam)

file(GLOB_RECURSE PROJECT_HEADERS header/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)


add_library(lib_slam STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        header/Omega.h)

target_link_libraries(lib_slam
        PUBLIC
        Eigen3::Eigen
        xtensor
        xtl
)

target_include_directories(lib_slam
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/submodules/external/xtl/include
        ${CMAKE_SOURCE_DIR}/submodules/external/xtensor/include
        ${EIGEN3_INCLUDE_DIRS})


add_subdirectory(test)