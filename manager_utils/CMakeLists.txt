cmake_minimum_required(VERSION 3.5.1)

project(manager_utils LANGUAGES CXX)

set(_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(_INC_DIR ${_BASE_DIR}/manager_utils)
set(_SRC_DIR ${_BASE_DIR}/../src)

add_library(
    ${PROJECT_NAME} STATIC # STATIC or SHARED
        ${_INC_DIR}/config/DrawMgrCfg.h
        ${_INC_DIR}/config/RsrcMgrCfg.h
        ${_INC_DIR}/config/ManagerHandlerCfg.h
        ${_INC_DIR}/managers/MgrBase.h
        ${_INC_DIR}/managers/DrawMgr.h
        ${_INC_DIR}/managers/RsrcMgr.h
        ${_INC_DIR}/managers/TimerMgr.h
        ${_INC_DIR}/managers/ManagerHandler.h
        ${_INC_DIR}/drawing/Widget.h
        ${_INC_DIR}/drawing/Image.h
        ${_INC_DIR}/drawing/Text.h
        ${_INC_DIR}/drawing/Fbo.h
        ${_INC_DIR}/input/ButtonBase.h
        ${_INC_DIR}/time/TimerData.h
        ${_INC_DIR}/time/TimerClient.h
        ${_INC_DIR}/debug/DebugConsole.h

        ${_SRC_DIR}/managers/DrawMgr.cpp
        ${_SRC_DIR}/managers/RsrcMgr.cpp
        ${_SRC_DIR}/managers/TimerMgr.cpp
        ${_SRC_DIR}/managers/ManagerHandler.cpp
        ${_SRC_DIR}/drawing/Widget.cpp
        ${_SRC_DIR}/drawing/Image.cpp
        ${_SRC_DIR}/drawing/Text.cpp
        ${_SRC_DIR}/drawing/Fbo.cpp
        ${_SRC_DIR}/input/ButtonBase.cpp
        ${_SRC_DIR}/time/TimerClient.cpp
        ${_SRC_DIR}/debug/DebugConsole.cpp
)

set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
        PUBLIC
            ${_BASE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
        PUBLIC
            sdl_utils
)