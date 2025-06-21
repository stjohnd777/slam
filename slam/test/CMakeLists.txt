cmake_minimum_required(VERSION 3.30)
project(test_slam)

file(GLOB_RECURSE PROJECT_HEADERS header/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0 # Use the latest stable release
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_slam ${PROJECT_HEADERS} ${PROJECT_SOURCES})
target_link_libraries(test_slam
        gtest_main
        lib_slam
        xtensor
        xtl
        Eigen3::Eigen
)

target_include_directories(test_slam
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/header
        ${CMAKE_SOURCE_DIR}/slam/header
)

