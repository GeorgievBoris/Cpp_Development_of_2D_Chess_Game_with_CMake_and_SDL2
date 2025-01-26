cmake_minimum_required(VERSION 3.5.1)

message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers/helpers.cmake)
message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

project(buttons_and_timers LANGUAGES CXX)

set(_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(_INC_DIR ${_BASE_DIR}/include)
set(_SRC_DIR ${_BASE_DIR}/src)

add_executable(${PROJECT_NAME}
                ${_INC_DIR}/common/CommonDefines.h

                ${_INC_DIR}/engine/config/EngineCfg.h
                ${_INC_DIR}/engine/Engine.h
                ${_INC_DIR}/engine/EngineCfgLoader.h
                ${_INC_DIR}/game/config/GameCfg.h
                ${_INC_DIR}/game/Game.h
                ${_INC_DIR}/game/entities/Hero.h
                ${_INC_DIR}/game/entities/Wheel.h
                ${_INC_DIR}/game/proxies/GameProxy.h
                ${_INC_DIR}/game/buttons/WheelButton.h

                ${_SRC_DIR}/engine/Engine.cpp
                ${_SRC_DIR}/engine/EngineCfgLoader.cpp
                ${_SRC_DIR}/game/Game.cpp
                ${_SRC_DIR}/game/entities/Hero.cpp
                ${_SRC_DIR}/game/entities/Wheel.cpp
                ${_SRC_DIR}/game/buttons/WheelButton.cpp
                ${_SRC_DIR}/main.cpp
)

set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
        PRIVATE
            ${_INC_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            manager_utils # write this down in the book: the moment we link to the "manager_utils" library, we will take everything that is PUBLIC in the "target_include_directories()" of "manager_utils"
)


if(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
        -DRELEASE_BUILD=1
)

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory

        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()

add_subdirectory(utils) # here the name of the folder must be added, NOT the name of the binary
add_subdirectory(${_BASE_DIR}/sdl_utils) # here the name of the folder must be added, NOT the name of the binary
add_subdirectory(${_BASE_DIR}/manager_utils) # here the name of the folder must be added, NOT the name of the binary