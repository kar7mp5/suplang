#include "lexer/lexer.h"
#include <cctype>
#include <map>

// 문자열을 TokenType으로 매핑하기 위한 맵
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
        currentChar = '\0'; // NUL 문자 (파일 끝 의미)
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
    size_t startPos = position;
    while (currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')) {
        ident += currentChar;
        advance();
    }

    if (keywords.count(ident)) {
        return {keywords[ident], ident};
    }
    return {TokenType::IDENTIFIER, ident};
}

Token Lexer::makeNumber() {
    std::string num = "";
    size_t startPos = position;
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
            case '=':
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
            case '+':
                advance();
                return {TokenType::PLUS, "+"};
            // ... 다른 연산자 및 구분자 추가 ...
        }
        
        // 알 수 없는 문자는 ILLEGAL 토큰으로 처리
        Token illegalToken = {TokenType::ILLEGAL, std::string(1, currentChar)};
        advance();
        return illegalToken;
    }
    return {TokenType::END_OF_FILE, ""};
}