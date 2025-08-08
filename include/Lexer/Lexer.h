#ifndef SUPLANG_LEXER_LEXER_H_
#define SUPLANG_LEXER_LEXER_H_

#include "Lexer/Token.h"

#include <string>

namespace suplang {

// The Lexer class is responsible for taking a source code string and turning
// it into a sequence of tokens.
class Lexer {
  public:
    // Constructor takes the source code string to be tokenized.
    explicit Lexer(const std::string &source);

    // Returns the next token from the source code.
    Token nextToken();

  private:
    // Moves the lexer's position to the next character.
    void advance();

    // Returns the character immediately after the current one without advancing.
    char peekChar();

    // Skips over any whitespace characters (spaces, tabs, newlines).
    void skipWhitespace();

    // Consumes a sequence of letters/digits as an identifier or keyword.
    Token makeIdentifier();

    // Consumes a sequence of digits as a number.
    Token makeNumber();

    const std::string source_;
    size_t position_ = 0;   // Current position in the source_ string.
    char current_char_ = 0; // The character at the current position.
};

} // namespace suplang

#endif // SUPLANG_LEXER_LEXER_H_