project(raytrace)
cmake_minimum_required(VERSION 3.5.1)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

