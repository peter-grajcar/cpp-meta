cmake_minimum_required(VERSION 3.16)
project(cpp_meta)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE files "src/*.cpp")

add_executable(cpp_meta ${files})
target_include_directories(cpp_meta PUBLIC include)