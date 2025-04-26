add_library(lex
    src/Token.cpp
    src/Lexer.cpp
)

target_include_directories(lex
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
) 