cmake_minimum_required(VERSION 3.10)

# 明确列出语法分析器源文件
set(SYNTAX_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syntax.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/grammar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/first_follow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_table.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
)

# 明确列出语法分析器头文件
set(SYNTAX_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/syntax.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/grammar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/first_follow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser_table.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/parser.h
)

# 创建语法分析器库
add_library(syntax_analyzer STATIC
    ${SYNTAX_SOURCES}
    ${SYNTAX_HEADERS}
)

# 设置包含目录
target_include_directories(syntax_analyzer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 设置编译选项
target_compile_options(syntax_analyzer PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)