#ifndef SUPLANG_PARSER_PARSER_H_
#define SUPLANG_PARSER_PARSER_H_

#include "AST/ASTNode.h"
#include "Lexer/Lexer.h"

#include <map>

namespace suplang {

// Enum for operator precedence used by the Pratt parser.
enum Precedence {
    LOWEST,
    EQUALS,      // ==, !=
    LESSGREATER, // >, <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
};

// The Parser class creates an Abstract Syntax Tree (AST) from a stream of
// tokens provided by the Lexer.
class Parser {
  public:
    explicit Parser(Lexer &lexer);

    // The main entry point for parsing, returns the root of the AST.
    std::unique_ptr<ProgramNode> parseProgram();

  private:
    // Consumes the current token and advances to the next one.
    void nextToken();

    // Asserts the type of the next token and advances if it matches.
    bool expectPeek(TokenType type);

    // --- Statement Parsers ---
    std::unique_ptr<StatementNode> parseStatement();
    std::unique_ptr<StatementNode> parseVarDeclStatement();
    std::unique_ptr<StatementNode> parseIfStatement();
    std::unique_ptr<BlockStatementNode> parseBlockStatement();
    std::unique_ptr<StatementNode> parseExpressionStatement();

    // --- Expression Parsers (Pratt Parser Implementation) ---
    std::unique_ptr<ExpressionNode> parseExpression(Precedence precedence);
    std::unique_ptr<ExpressionNode> parseIdentifier();
    std::unique_ptr<ExpressionNode> parseIntegerLiteral();
    std::unique_ptr<ExpressionNode> parseBoolean();
    std::unique_ptr<ExpressionNode> parsePrefixExpression();
    std::unique_ptr<ExpressionNode> parseInfixExpression(std::unique_ptr<ExpressionNode> left);

    Lexer &lexer_;
    Token current_token_;
    Token peek_token_;
    std::map<TokenType, Precedence> precedences_;
};

} // namespace suplang

#endif // SUPLANG_PARSER_PARSER_H_