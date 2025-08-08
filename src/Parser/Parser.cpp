#include "Parser/Parser.h"
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    nextToken();
    nextToken();
}

void Parser::nextToken() {
    currentToken = peekToken;
    peekToken = lexer.nextToken();
}

bool Parser::expectPeek(TokenType type) {
    if (peekToken.type == type) {
        nextToken();
        return true;
    }
    std::cerr << "Parser Error: Expected next token to be " << (int)type << ", got " << (int)peekToken.type << " instead." << std::endl;
    return false;
}

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();
    while (currentToken.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    return program;
}

std::unique_ptr<StatementNode> Parser::parseStatement() {
    switch (currentToken.type) {
        case TokenType::INT32:
        case TokenType::FLOAT:
        case TokenType::BOOL:
        case TokenType::CHAR:
        case TokenType::LIST:
            return parseVarDeclStatement();
        default:
            return nullptr;
    }
}

std::unique_ptr<StatementNode> Parser::parseVarDeclStatement() {
    std::string type = currentToken.value;

    if (!expectPeek(TokenType::IDENTIFIER)) return nullptr;
    std::string name = currentToken.value;

    if (!expectPeek(TokenType::ASSIGN)) return nullptr;
    
    nextToken();

    auto value = parseExpression();

    if (peekToken.type == TokenType::SEMICOLON) {
        nextToken();
    }
    
    return std::make_unique<VarDeclNode>(type, name, std::move(value));
}

std::unique_ptr<ExpressionNode> Parser::parseExpression() {
    if (currentToken.type == TokenType::INTEGER_LITERAL) {
        return parseIntegerLiteral();
    }
    return nullptr;
}

std::unique_ptr<ExpressionNode> Parser::parseIntegerLiteral() {
    int32_t value = std::stoi(currentToken.value);
    return std::make_unique<NumberLiteralNode>(value);
}