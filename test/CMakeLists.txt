cmake_minimum_required(VERSION 3.5.1)

# to be able to test with some framework - it is not magic. We either have it installed or have it as source code.
# When using Google Framework, there are two variants. 1) Check it out inside our project and compile it as a part of our
# project or 2) CMake has a very cool option to patch / fetch some content from Internet and build the project with it.
# With option 2) it is not necessary to have each time their source code in our system. We are going to use the simpler variant,
# so we are gonna just copy it.

# In the terminal, enter the test folder with command "cd test" and type "git clone https://github.com/google/googletest/"
# a folder "googletest" appears inside the "test" folder. This folder contains its own CMakeLists.txt file. Do I need to know...
# how to build this framework. Answer is No.

add_subdirectory(googletest) # CMake automatically is gonna handle it and build it

# libgtest and libgmock -> these are two libraries. Google Test and Google Mock (this one is for more advanced tests)...
# each of them has one static library -> libgtest_main and libgmock_main - they are used for linking


# this is marco that will turn on/enable all tests only if the marco is called 
enable_testing()

add_subdirectory(utils)

# # gtest_SOURCE_DIR - this is variable filled with the source directory path
# # we give it as an include directory to our applicaiton. Because when it starts to include source files of Google Test...
# # it will say what are these files, I do not know where they are located. Well, they are located in "gtest_SOURCE_DIR/include"
# ${gtest_SOURCE_DIR}/include 