#ifndef SUPLANG_PARSER_PARSER_H_
#define SUPLANG_PARSER_PARSER_H_

#include "AST/ASTNode.h"
#include "Lexer/Lexer.h"

#include <map>
#include <vector>

namespace suplang {

// Enum for operator precedence used by the Pratt parser.
enum Precedence {
    LOWEST,
    EQUALS,      // ==, !=
    LESSGREATER, // >, <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL,        // myFunction(X)
};

class Parser {
  public:
    explicit Parser(Lexer &lexer);
    std::unique_ptr<ProgramNode> parseProgram();

  private:
    void nextToken();
    bool expectPeek(TokenType type);

    // Statement parsers.
    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<StatementNode> parseVarDeclStatement();
    std::unique_ptr<StatementNode> parseIfStatement();
    std::unique_ptr<StatementNode> parseWhileStatement(); // New parser method.
    std::unique_ptr<BlockStatementNode> parseBlockStatement();
    std::unique_ptr<StatementNode> parseExpressionStatement();
    std::unique_ptr<StatementNode> parseReturnStatement();

    // Expression parsers.
    std::unique_ptr<ExpressionNode> parseExpression(Precedence precedence);
    std::unique_ptr<ExpressionNode> parseIdentifier();
    std::unique_ptr<ExpressionNode> parseIntegerLiteral();
    std::unique_ptr<ExpressionNode> parseBoolean();
    std::unique_ptr<ExpressionNode> parsePrefixExpression();
    std::unique_ptr<ExpressionNode> parseInfixExpression(std::unique_ptr<ExpressionNode> left);
    std::unique_ptr<ExpressionNode> parseFunctionLiteral();
    std::vector<Parameter> parseFunctionParameters();
    std::unique_ptr<ExpressionNode> parseCallExpression(std::unique_ptr<ExpressionNode> function);
    std::vector<std::unique_ptr<ExpressionNode>> parseCallArguments();

    Lexer &lexer_;
    Token current_token_;
    Token peek_token_;
    std::map<TokenType, Precedence> precedences_;
};

} // namespace suplang

#endif // SUPLANG_PARSER_PARSER_H_