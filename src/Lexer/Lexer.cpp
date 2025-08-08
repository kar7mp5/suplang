#include "Lexer/Lexer.h"

#include <cctype>
#include <map>

namespace suplang {

namespace {

// Map of keywords to their corresponding token types for quick lookup.
const std::map<std::string, TokenType> kKeywords = {
    {"def", TokenType::DEF},       {"class", TokenType::CLASS}, {"struct", TokenType::STRUCT},
    {"return", TokenType::RETURN}, {"int32", TokenType::INT32}, {"float", TokenType::FLOAT},
    {"bool", TokenType::BOOL},     {"char", TokenType::CHAR},   {"list", TokenType::LIST},
    {"if", TokenType::IF},         {"elif", TokenType::ELIF},   {"else", TokenType::ELSE},
    {"true", TokenType::TRUE},     {"false", TokenType::FALSE}, {"while", TokenType::WHILE}, // Added while keyword.
};

} // namespace

Lexer::Lexer(const std::string &source) : source_(source) {
    if (!source_.empty()) {
        current_char_ = source_[position_];
    } else {
        current_char_ = 0;
    }
}

// Other Lexer methods (advance, peekChar, skipWhitespace, etc.) are unchanged.
// ... (rest of the file is identical to the previous version) ...

// NOTE: For brevity, the unchanged parts of Lexer.cpp are omitted.
// The only change is adding "while" to the kKeywords map.
// The full file content from the previous response is still valid.
// The provided stub is for context.

void Lexer::advance() {
    position_++;
    if (position_ >= source_.length()) {
        current_char_ = 0;
    } else {
        current_char_ = source_[position_];
    }
}

char Lexer::peekChar() {
    if (position_ + 1 >= source_.length()) {
        return 0;
    }
    return source_[position_ + 1];
}

void Lexer::skipWhitespace() {
    while (current_char_ != 0 && isspace(current_char_)) {
        advance();
    }
}

Token Lexer::makeIdentifier() {
    std::string ident;
    while (current_char_ != 0 && (isalnum(current_char_) || current_char_ == '_')) {
        ident += current_char_;
        advance();
    }

    if (kKeywords.count(ident)) {
        return {kKeywords.at(ident), ident};
    }
    return {TokenType::IDENTIFIER, ident};
}

Token Lexer::makeNumber() {
    std::string num;
    while (current_char_ != 0 && isdigit(current_char_)) {
        num += current_char_;
        advance();
    }
    return {TokenType::INTEGER_LITERAL, num};
}

Token Lexer::nextToken() {
    while (current_char_ != 0) {
        if (isspace(current_char_)) {
            skipWhitespace();
            continue;
        }
        if (isalpha(current_char_) || current_char_ == '_') {
            return makeIdentifier();
        }
        if (isdigit(current_char_)) {
            return makeNumber();
        }

        switch (current_char_) {
        case '=':
            if (peekChar() == '=') {
                advance();
                advance();
                return {TokenType::EQUALS, "=="};
            }
            advance();
            return {TokenType::ASSIGN, "="};
        case ';':
            advance();
            return {TokenType::SEMICOLON, ";"};
        case '(':
            advance();
            return {TokenType::LPAREN, "("};
        case ')':
            advance();
            return {TokenType::RPAREN, ")"};
        case '{':
            advance();
            return {TokenType::LBRACE, "{"};
        case '}':
            advance();
            return {TokenType::RBRACE, "}"};
        case ',':
            advance();
            return {TokenType::COMMA, ","};
        case '+':
            advance();
            return {TokenType::PLUS, "+"};
        case '-':
            advance();
            return {TokenType::MINUS, "-"};
        case '*':
            advance();
            return {TokenType::ASTERISK, "*"};
        case '/':
            advance();
            return {TokenType::SLASH, "/"};
        case '<':
            advance();
            return {TokenType::LT, "<"};
        case '>':
            advance();
            return {TokenType::GT, ">"};
        case '!':
            if (peekChar() == '=') {
                advance();
                advance();
                return {TokenType::NOT_EQUALS, "!="};
            }
            break;
        }

        Token illegal_token = {TokenType::ILLEGAL, std::string(1, current_char_)};
        advance();
        return illegal_token;
    }
    return {TokenType::END_OF_FILE, ""};
}

} // namespace suplang