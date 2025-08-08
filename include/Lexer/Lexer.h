#pragma once
#include "Token.h"

class Lexer {
public:
    // Constructor takes the source code string
    Lexer(const std::string& source);

    // Returns the next token from the source code
    Token nextToken();

private:
    std::string source;
    size_t position = 0; // Current position in the source string
    char currentChar;    // The character at the current position

    void advance();           // Moves to the next character
    void skipWhitespace();    // Skips over whitespace characters
    Token makeIdentifier();   // Creates an IDENTIFIER or KEYWORD token
    Token makeNumber();       // Creates an INTEGER_LITERAL token
};