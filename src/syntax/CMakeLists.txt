# src/syntax/CMakeLists.txt
file(GLOB SYNTAX_SOURCES *.cpp)

# 创建语法分析库
add_library(syntax_lib STATIC
    ${SYNTAX_SOURCES}
)

target_include_directories(syntax_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include)