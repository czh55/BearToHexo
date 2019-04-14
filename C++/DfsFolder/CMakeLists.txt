cmake_minimum_required(VERSION 2.8.12)

PROJECT(DfsFolder)

set(CMAKE_CXX_FLAGS "-g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories( ${CMAKE_CURRENT_LIST_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)


