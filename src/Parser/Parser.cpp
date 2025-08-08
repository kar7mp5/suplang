#include "Parser/Parser.h"
#include <iostream>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    // Setup operator precedences
    precedences = {
        {TokenType::EQUALS, Precedence::EQUALS},
        {TokenType::NOT_EQUALS, Precedence::EQUALS},
        {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},
        {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},
        {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},
    };
    
    // Initialize currentToken and peekToken
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
    
    nextToken(); // Move past '=' to the expression

    auto value = parseExpression(Precedence::LOWEST);

    if (peekToken.type == TokenType::SEMICOLON) {
        nextToken();
    }
    
    return std::make_unique<VarDeclNode>(type, name, std::move(value));
}

std::unique_ptr<ExpressionNode> Parser::parseExpression(Precedence precedence) {
    // Prefix parsing
    std::unique_ptr<ExpressionNode> leftExp;
    switch (currentToken.type) {
        case TokenType::IDENTIFIER:
            leftExp = parseIdentifier();
            break;
        case TokenType::INTEGER_LITERAL:
            leftExp = parseIntegerLiteral();
            break;
        case TokenType::MINUS:
             leftExp = parsePrefixExpression();
             break;
        default:
            return nullptr;
    }

    // Infix parsing
    while (peekToken.type != TokenType::SEMICOLON && precedence < (precedences.count(peekToken.type) ? precedences[peekToken.type] : Precedence::LOWEST)) {
        nextToken();
        leftExp = parseInfixExpression(std::move(leftExp));
    }

    return leftExp;
}

std::unique_ptr<ExpressionNode> Parser::parseIdentifier() {
    return std::make_unique<IdentifierNode>(currentToken.value);
}

std::unique_ptr<ExpressionNode> Parser::parseIntegerLiteral() {
    int32_t value = std::stoi(currentToken.value);
    return std::make_unique<NumberLiteralNode>(value);
}

std::unique_ptr<ExpressionNode> Parser::parsePrefixExpression() {
    std::string op = currentToken.value;
    nextToken();
    auto right = parseExpression(Precedence::PREFIX);
    return std::make_unique<PrefixExpressionNode>(op, std::move(right));
}

std::unique_ptr<ExpressionNode> Parser::parseInfixExpression(std::unique_ptr<ExpressionNode> left) {
    std::string op = currentToken.value;
    Precedence currentPrecedence = precedences[currentToken.type];
    nextToken();
    auto right = parseExpression(currentPrecedence);
    return std::make_unique<InfixExpressionNode>(std::move(left), op, std::move(right));
}