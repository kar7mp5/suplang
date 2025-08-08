#ifndef SUPLANG_LEXER_TOKEN_H_
#define SUPLANG_LEXER_TOKEN_H_

#include <string>

namespace suplang {

// Represents the type of a token that the Lexer can produce.
enum class TokenType {
    // Keywords
    CLASS,
    STRUCT,
    IF,
    ELIF,
    ELSE,
    RETURN,
    INT32,
    FLOAT,
    BOOL,
    CHAR,
    LIST,
    TRUE,
    FALSE,
    DEF,
    WHILE, // New keyword for while loops.

    // Identifiers & Literals
    IDENTIFIER,
    INTEGER_LITERAL,

    // Operators
    ASSIGN,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    EQUALS,
    NOT_EQUALS,
    LT,
    GT,

    // Delimiters
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON,
    COMMA,

    // Other
    ILLEGAL,
    END_OF_FILE,
};

// Represents a single token.
struct Token {
    TokenType type;
    std::string value;
};

} // namespace suplang

#endif // SUPLANG_LEXER_TOKEN_H_