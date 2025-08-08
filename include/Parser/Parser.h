#pragma once
#include "Lexer/Lexer.h"
#include "AST/ASTNode.h"

class Parser {
public:
    Parser(Lexer& lexer);
    std::unique_ptr<ProgramNode> parseProgram();

private:
    Lexer& lexer;
    Token currentToken;
    Token peekToken;

    void nextToken();
    bool expectPeek(TokenType type);

    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<StatementNode> parseVarDeclStatement();
    std::unique_ptr<ExpressionNode> parseExpression();
    std::unique_ptr<ExpressionNode> parseIntegerLiteral();
};