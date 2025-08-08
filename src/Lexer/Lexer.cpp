#include "Lexer/Lexer.h"
#include <cctype>
#include <map>

// Map of keywords to their corresponding token types
std::map<std::string, TokenType> keywords = {
    {"def", TokenType::DEF}, {"class", TokenType::CLASS},
    {"struct", TokenType::STRUCT}, {"if", TokenType::IF},
    {"else", TokenType::ELSE}, {"while", TokenType::WHILE},
    {"for", TokenType::FOR}, {"return", TokenType::RETURN},
    {"int32", TokenType::INT32}, {"float", TokenType::FLOAT},
    {"bool", TokenType::BOOL}, {"char", TokenType::CHAR},
    {"list", TokenType::LIST}, {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}
};

Lexer::Lexer(const std::string& source) : source(source) {
    if (!source.empty()) {
        currentChar = source[position];
    } else {
        currentChar = '\0'; // NUL character signifies end of file
    }
}

void Lexer::advance() {
    position++;
    if (position >= source.length()) {
        currentChar = '\0';
    } else {
        currentChar = source[position];
    }
}

void Lexer::skipWhitespace() {
    while (currentChar != '\0' && isspace(currentChar)) {
        advance();
    }
}

Token Lexer::makeIdentifier() {
    std::string ident = "";
    while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
        ident += currentChar;
        advance();
    }

    // Check if the identifier is a keyword
    if (keywords.count(ident)) {
        return {keywords[ident], ident};
    }
    return {TokenType::IDENTIFIER, ident};
}

Token Lexer::makeNumber() {
    std::string num = "";
    while (currentChar != '\0' && isdigit(currentChar)) {
        num += currentChar;
        advance();
    }
    return {TokenType::INTEGER_LITERAL, num};
}

Token Lexer::nextToken() {
    while (currentChar != '\0') {
        if (isspace(currentChar)) {
            skipWhitespace();
            continue;
        }
        if (isalpha(currentChar) || currentChar == '_') {
            return makeIdentifier();
        }
        if (isdigit(currentChar)) {
            return makeNumber();
        }

        switch (currentChar) {
            case '=': advance(); return {TokenType::ASSIGN, "="};
            case ';': advance(); return {TokenType::SEMICOLON, ";"};
            case '(': advance(); return {TokenType::LPAREN, "("};
            case ')': advance(); return {TokenType::RPAREN, ")"};
            case '{': advance(); return {TokenType::LBRACE, "{"};
            case '}': advance(); return {TokenType::RBRACE, "}"};
            case '+': advance(); return {TokenType::PLUS, "+"};
            case '-': advance(); return {TokenType::MINUS, "-"};
            case '*': advance(); return {TokenType::ASTERISK, "*"};
            case '/': advance(); return {TokenType::SLASH, "/"};
        }
        
        // If the character is unknown, return an ILLEGAL token
        Token illegalToken = {TokenType::ILLEGAL, std::string(1, currentChar)};
        advance();
        return illegalToken;
    }
    return {TokenType::END_OF_FILE, ""};
}