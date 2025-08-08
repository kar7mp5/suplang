#pragma once
#include <string>
#include <vector>

// Enum class representing all possible token types
enum class TokenType {
    // Keywords
    DEF,
    CLASS,
    STRUCT,
    RETURN,
    INT32,
    FLOAT,
    BOOL,
    CHAR,
    LIST,

    // New keywords for control flow and boolean values
    IF,
    ELIF,
    ELSE,
    TRUE,
    FALSE,

    // Identifiers and Literals
    IDENTIFIER,
    INTEGER_LITERAL,
    STRING_LITERAL,

    // Operators
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    PERCENT,
    ASSIGN,
    INCREMENT,
    DECREMENT,

    // New comparison operators
    EQUALS,
    NOT_EQUALS, // ==, !=
    LT,
    GT, // <, >

    // Delimiters
    LPAREN,
    RPAREN, // ( )
    LBRACE,
    RBRACE, // { }
    LBRACKET,
    RBRACKET, // [ ]
    SEMICOLON,
    COLON,
    DOT,
    COMMA,

    // Other
    ILLEGAL, // Represents an unknown token
    END_OF_FILE
};

// Represents a single token with its type and string value
struct Token {
    TokenType type;
    std::string value;
};