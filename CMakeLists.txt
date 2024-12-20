cmake_minimum_required(VERSION 3.5.1)

message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers/helpers.cmake)
message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

project(input_events LANGUAGES CXX)
#project(sdl_runtime) # LANGUAGES CXX could be important in some caes

set(_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    set(SDL2_image_name SDL2_image)
    set(SDL2_ttf_name SDL2_ttf)
    set(SDL2_mixer_name SDL2_mixer)
elseif(WIN32 OR APPLE)
    set(SDL2_image_name SDL2_IMAGE)
    set(SDL2_ttf_name SDL2_TTF)
    set(SDL2_mixer_name SDL2_MIXER)
endif()

find_package(SDL2 REQUIRED)
find_package(${SDL2_image_name} REQUIRED)
find_package(${SDL2_ttf_name} REQUIRED)
find_package(${SDL2_mixer_name} REQUIRED)

add_executable(${PROJECT_NAME}
                ${_BASE_DIR}/engine/Engine.h
                ${_BASE_DIR}/engine/Engine.cpp

                ${_BASE_DIR}/utils/drawing/Rectangle.h
                ${_BASE_DIR}/utils/drawing/Rectangle.cpp
                ${_BASE_DIR}/utils/drawing/Point.h
                ${_BASE_DIR}/utils/drawing/Point.cpp
                ${_BASE_DIR}/utils/thread/ThreadUtils.h
                ${_BASE_DIR}/utils/thread/ThreadUtils.cpp
                ${_BASE_DIR}/utils/time/Time.h
                ${_BASE_DIR}/utils/time/Time.cpp
                
                ${_BASE_DIR}/sdl_utils/SDLLoader.cpp
                ${_BASE_DIR}/sdl_utils/SDLLoader.h
                ${_BASE_DIR}/sdl_utils/config/MonitorWindowCfg.h
                ${_BASE_DIR}/sdl_utils/MonitorWindow.cpp
                ${_BASE_DIR}/sdl_utils/MonitorWindow.h
                ${_BASE_DIR}/sdl_utils/Texture.cpp
                ${_BASE_DIR}/sdl_utils/Texture.h
                ${_BASE_DIR}/sdl_utils/InputEvent.cpp
                ${_BASE_DIR}/sdl_utils/InputEvent.h
                

                ${_BASE_DIR}/main.cpp
)

set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
        PRIVATE
            ${_BASE_DIR}
            ${SDL2_INCLUDE_DIR}
            ${SDL2_IMAGE_INCLUDE_DIR}
            ${SDL2_TTF_INCLUDE_DIR}
            ${SDL2_MIXER_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARY}
            ${SDL2_TTF_LIBRARY}
            ${SDL2_MIXER_LIBRARY}
)