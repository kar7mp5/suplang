#pragma once
#include <string>
#include <vector>

enum class TokenType {
    // Keyword
    DEF, CLASS, STRUCT, IF, ELSE, WHILE, FOR, RETURN,
    INT32, FLOAT, BOOL, CHAR, LIST, TRUE, FALSE,

    // Identifier, Literal
    IDENTIFIER,
    INTEGER_LITERAL,
    STRING_LITERAL,

    // Operator 
    PLUS, MINUS, ASTERISK, SLASH, PERCENT,
    ASSIGN, EQUALS, NOT_EQUALS,
    LT, GT, LTE, GTE,
    INCREMENT, DECREMENT,

    // Seperator
    LPAREN, RPAREN, // ( )
    LBRACE, RBRACE, // { }
    LBRACKET, RBRACKET, // [ ]
    SEMICOLON, COLON, DOT, COMMA,

    // ETC
    ILLEGAL,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
};