cmake_minimum_required(VERSION 3.21)
project(piv_c)

set(CMAKE_CXX_STANDARD 14)

add_executable(piv_c main.cpp)

# for OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(piv_c ${OpenCV_LIBS})

# for Boost
find_package(Boost 1.57.0 COMPONENTS thread filesystem system coroutine context chrono container)
include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
target_link_libraries(piv_c ${Boost_LIBRARIES})
