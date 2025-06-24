cmake_minimum_required(VERSION 3.30)
project(main_slam)

file(GLOB_RECURSE PROJECT_HEADERS header/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)


add_executable(main ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        src/main_graph_slam.cpp)

target_link_libraries(main
        PRIVATE
        lib_slam
        Eigen3::Eigen
        xtensor
        xtl)
