cmake_minimum_required(VERSION 3.10)
project(tiny-compiler CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录为bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加include目录
include_directories(include)

# 添加子目录
add_subdirectory(src)
