cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(SOURCES
    src/main.cpp
    src/Math/Math.cpp
)

add_executable(suplang ${SOURCES})

target_include_directories(suplang PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)