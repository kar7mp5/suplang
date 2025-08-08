#pragma once
#include <string>
#include <vector>

// Enum class representing all possible token types
enum class TokenType {
    // Keywords
    DEF, CLASS, STRUCT, IF, ELSE, WHILE, FOR, RETURN,
    INT32, FLOAT, BOOL, CHAR, LIST, TRUE, FALSE,

    // Identifiers and Literals
    IDENTIFIER,
    INTEGER_LITERAL,
    STRING_LITERAL,

    // Operators
    PLUS, MINUS, ASTERISK, SLASH, PERCENT,
    ASSIGN, EQUALS, NOT_EQUALS,
    LT, GT, LTE, GTE,
    INCREMENT, DECREMENT,

    // Delimiters
    LPAREN, RPAREN, // ( )
    LBRACE, RBRACE, // { }
    LBRACKET, RBRACKET, // [ ]
    SEMICOLON, COLON, DOT, COMMA,

    // Other
    ILLEGAL,     // Represents an unknown token
    END_OF_FILE
};

// Represents a single token with its type and string value
struct Token {
    TokenType type;
    std::string value; // The actual string slice from the source code
};