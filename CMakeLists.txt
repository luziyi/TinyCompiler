cmake_minimum_required(VERSION 3.10)
project(TinyCompiler VERSION 1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录为根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# 添加包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/lex/include
    ${CMAKE_SOURCE_DIR}/syntax/include
)

# 添加子目录
add_subdirectory(lex)
add_subdirectory(syntax)

# 收集源文件
set(SOURCES
    main.cpp
)

# 创建可执行文件
add_executable(compiler ${SOURCES})

# 链接词法和语法分析器库
target_link_libraries(compiler
    lexical_analyzer
    syntax_analyzer
)

# 安装设置
install(TARGETS compiler
    RUNTIME DESTINATION .
)

# 数据文件夹
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data)