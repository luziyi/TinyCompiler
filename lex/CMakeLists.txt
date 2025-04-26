cmake_minimum_required(VERSION 3.10)

# 收集词法分析器源文件
file(GLOB LEX_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# 收集词法分析器头文件
file(GLOB LEX_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)

# 创建词法分析器库
add_library(lexical_analyzer STATIC
    ${LEX_SOURCES}
    ${LEX_HEADERS}
)

# 设置包含目录
target_include_directories(lexical_analyzer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 设置编译选项
target_compile_options(lexical_analyzer PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)