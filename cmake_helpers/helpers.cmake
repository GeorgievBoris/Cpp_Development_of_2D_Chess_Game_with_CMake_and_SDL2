cmake_minimum_required(VERSION 3.10.2)


# # NOTE: The below list(...) is the original command as provided by Zhivko, but he ...
# # ... has made a small typo related to the system path ...
# # ... therefore I rewrote the command with the correct system path

# list(APPEND 
#     CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/find_modules)

list(APPEND 
    CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers/find_modules)

# # try with "set()" instead of with "list(APPEND)"

# set(CMAKE_MODULE_PATH 
#     ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers/find_modules)

function(set_target_cpp_standard target standard)
set_target_properties(
    ${target}
    PROPERTIES
        CXX_STANDARD ${standard}
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
endfunction()

function(enable_target_warnings target)
    target_compile_options(
        ${target}
        PRIVATE
          -Wall
          -Wextra
          -Werror
          -Wundef
          -Wuninitialized
          -Wreorder
          -Wshadow
          -Wpointer-arith
          -Wcast-align
          -Wcast-qual
          -Wconversion
          -Wunused-parameter
          -Wlogical-op
          -Wdouble-promotion
          -Wnon-virtual-dtor
          -Woverloaded-virtual
          -Wduplicated-cond
          -Wduplicated-branches
          -Wnull-dereference
    )
endfunction()
