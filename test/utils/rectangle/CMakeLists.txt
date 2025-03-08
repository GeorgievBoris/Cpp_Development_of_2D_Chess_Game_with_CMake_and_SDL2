cmake_minimum_required(VERSION 3.5.1)

project(rectangle_test)

add_executable(
    ${PROJECT_NAME}
        main.cpp
        RectangleTest.cpp
        RectangleTest.h
)

#include header files of the GoogleTest library
target_include_directories(
    ${PROJECT_NAME}
        PRIVATE
            ${gtest_SOURCE_DIR}/include
)

# must link to the utils library since we need the Rectangle that is lives inside utils
target_link_libraries(
    ${PROJECT_NAME}
        PRIVATE
            # v momenta v koito linknem kum edna biblioteka, tq si nosi sobstvenite "includes" sus sebe si...
            # v sluchaq utils bibliotekata v neinia CMakeLists.txt kazva "target_include_directories()" i si include-va svoq include path
            utils 
            # must also link to G-Test - because it does not have only headers, but also has source files and we need these source files
            gtest # a library
            gtest_main # this is the way how they release code with a "main.cpp" file 
)   

# ponezhe tova e otdelno binary, zadulzhitelno trqbva da se pusnat warning-ite i da se set-ne standarta...
# ako ne set-nem standarta shte se izpolzva default-en standart ot nashia compiler, koito mozhe da e nai-veroiatno C++11...
# samo che kogato ostatuka ot prilozhenieto e C++17, veche v takuv sluchai bihme imali nesuvmestimost
set_target_cpp_standard(${PROJECT_NAME} 17)
enable_target_warnings(${PROJECT_NAME})

# marco e CMake funkcia
# kato v CMake ima 2 neshta -> function i macro - razlikata e che ednoto vadi localnite promenlivi vunka, a drugoto gi durzheshe private

add_test(${PROJECT_NAME} ${PROJECT_NAME}) # this is another option
# add_test("rectangle_name" ${PROJECT_NAME}) # this is another option