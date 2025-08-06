#pragma once // 인클루드 가드 (C++11 이상)
#include <string>
#include <vector>

// 토큰의 종류를 나타내는 열거형
enum class TokenType {
    // 키워드
    DEF, CLASS, STRUCT, IF, ELSE, WHILE, FOR, RETURN,
    INT32, FLOAT, BOOL, CHAR, LIST, TRUE, FALSE,

    // 식별자, 리터럴
    IDENTIFIER,
    INTEGER_LITERAL,
    STRING_LITERAL,

    // 연산자
    PLUS, MINUS, ASTERISK, SLASH, PERCENT,
    ASSIGN, EQUALS, NOT_EQUALS,
    LT, GT, LTE, GTE,
    INCREMENT, DECREMENT,

    // 구분자
    LPAREN, RPAREN, // ( )
    LBRACE, RBRACE, // { }
    LBRACKET, RBRACKET, // [ ]
    SEMICOLON, COLON, DOT, COMMA,

    // 기타
    ILLEGAL, // 알 수 없는 토큰
    END_OF_FILE
};

// 토큰 구조체
struct Token {
    TokenType type;
    std::string value; // 토큰의 실제 문자열 값 (예: "my_var", "123")
};