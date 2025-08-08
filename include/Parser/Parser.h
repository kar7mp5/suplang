#pragma once
#include "Lexer/Lexer.h"
#include "AST/ASTNode.h"
#include <map>

// Enum for operator precedence
enum Precedence {
    LOWEST,
    EQUALS,      // ==
    LESSGREATER, // > or <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL         // myFunction(X)
};

class Parser {
public:
    Parser(Lexer& lexer);
    std::unique_ptr<ProgramNode> parseProgram();

private:
    Lexer& lexer;
    Token currentToken;
    Token peekToken;
    std::map<TokenType, Precedence> precedences;

    void nextToken();
    bool expectPeek(TokenType type);

    // Statement parsers
    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<StatementNode> parseVarDeclStatement();

    // Expression parsers (Pratt Parser implementation)
    std::unique_ptr<ExpressionNode> parseExpression(Precedence precedence);
    std::unique_ptr<ExpressionNode> parseIdentifier();
    std::unique_ptr<ExpressionNode> parseIntegerLiteral();
    std::unique_ptr<ExpressionNode> parsePrefixExpression();
    std::unique_ptr<ExpressionNode> parseInfixExpression(std::unique_ptr<ExpressionNode> left);
};