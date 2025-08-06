#include <iostream>
#include "lexer/lexer.h"

// TokenType을 문자열로 변환해주는 도우미 함수 (출력용)
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::DEF: return "DEF";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INT32: return "INT32";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::END_OF_FILE: return "EOF";
        default: return "UNKNOWN";
    }
}

int main() {
    std::string code = "int32 x = 5; def myFunction() {}";
    Lexer lexer(code);

    Token token;
    do {
        token = lexer.nextToken();
        std::cout << "Type: " << tokenTypeToString(token.type)
                  << ", Value: '" << token.value << "'" << std::endl;
    } while (token.type != TokenType::END_OF_FILE);

    return 0;
}