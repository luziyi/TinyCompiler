enable_testing()

# 添加词法分析器测试（使用Google Test框架）
add_executable(lexer_gtest
    LexerTest.cpp
)
target_link_libraries(lexer_gtest
    PRIVATE
        lex
        GTest::gtest_main
)

# 添加语法分析器测试
add_executable(parser_test
    ParserTest.cpp
)
target_link_libraries(parser_test
    PRIVATE
        syntax
        lex
        GTest::gtest
)

# 向CMake的测试系统添加测试
add_test(NAME lexer_test COMMAND lexer_gtest)
add_test(NAME parser_test COMMAND parser_test)

# 简单的词法分析器测试
# add_executable(lexer_test 
#     LexerTest.cpp
# )
# target_link_libraries(lexer_test 
#     PRIVATE
#         lex
# )

# 语法分析器测试
# 暂时注释掉，因为ParserTest.cpp文件不存在
# add_executable(parser_test 
#     ParserTest.cpp
# )
# target_link_libraries(parser_test 
#     PRIVATE
#         syntax
#         lex
# ) 