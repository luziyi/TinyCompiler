# src/lexical/CMakeLists.txt
file(GLOB LEXICAL_SOURCES *.cpp)

# 创建词法分析库
add_library(lexical_lib STATIC
    ${LEXICAL_SOURCES}
)

target_include_directories(lexical_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)