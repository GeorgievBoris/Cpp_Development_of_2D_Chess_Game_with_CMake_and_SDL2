cmake_minimum_required(VERSION 3.5.1)

project(utils LANGUAGES CXX)

set(_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(_SRC_DIR ${_BASE_DIR}/../src)
set(_INC_DIR ${_BASE_DIR}/utils)

add_library(
    ${PROJECT_NAME} STATIC # STATIC or SHARED

    ${_INC_DIR}/drawing/Rectangle.h
    ${_INC_DIR}/drawing/Point.h
    ${_INC_DIR}/drawing/DrawParams.h
    ${_INC_DIR}/drawing/Color.h
    ${_INC_DIR}/thread/ThreadUtils.h
    ${_INC_DIR}/time/Time.h

    ${_SRC_DIR}/drawing/Rectangle.cpp
    ${_SRC_DIR}/drawing/Point.cpp
    ${_SRC_DIR}/drawing/Color.cpp
    ${_SRC_DIR}/thread/ThreadUtils.cpp
    ${_SRC_DIR}/time/Time.cpp
)

set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
        PUBLIC
            ${_BASE_DIR}
)