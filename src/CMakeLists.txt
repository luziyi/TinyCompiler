# src/CMakeLists.txt
add_subdirectory(lexical)
add_subdirectory(syntax)

# 创建主可执行文件
add_executable(tiny-compiler 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# 链接子目录的库
target_link_libraries(tiny-compiler 
    lexical_lib
    syntax_lib
)