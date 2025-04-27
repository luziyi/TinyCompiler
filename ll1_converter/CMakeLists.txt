cmake_minimum_required(VERSION 3.10)

# 确保在project命令之前设置编译器
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(LL1Converter)

# 设置 C++ 标准 - 修改为 C++17 以支持结构化绑定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译器选项
if(MSVC)
    # MSVC编译选项
    add_compile_options(/W4)
else()
    # GCC/Clang编译选项
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# 添加可执行文件
add_executable(ll1_converter 
    main.cpp
    Grammar.cpp
    LL1Converter.cpp
)

# 设置包含目录
target_include_directories(ll1_converter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 打印调试信息
message(STATUS "LL1Converter配置完成")