#include "Parser/Parser.h"

#include <iostream>

namespace suplang {

Parser::Parser(Lexer &lexer) : lexer_(lexer) {
    // Sets up the precedence table for infix operators. A higher value
    // indicates higher precedence.
    precedences_ = {
        {TokenType::ASSIGN, Precedence::EQUALS},     {TokenType::EQUALS, Precedence::EQUALS},
        {TokenType::NOT_EQUALS, Precedence::EQUALS}, {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},    {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},         {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},
    };

    // Initializes the parser by reading the first two tokens, so that both
    // current_token_ and peek_token_ are populated.
    nextToken();
    nextToken();
}

void Parser::nextToken() {
    current_token_ = peek_token_;
    peek_token_ = lexer_.nextToken();
}

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();
    // Continue parsing statements until the end of the file is reached.
    while (current_token_.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        // Advance to the next token to start parsing the next statement.
        nextToken();
    }
    return program;
}

// Private helper methods for parsing different language constructs.

std::unique_ptr<StatementNode> Parser::parseStatement() {
    switch (current_token_.type) {
    case TokenType::INT32:
    case TokenType::FLOAT:
    case TokenType::BOOL:
    case TokenType::CHAR:
    case TokenType::LIST:
        return parseVarDeclStatement();
    case TokenType::IF:
        return parseIfStatement();
    default:
        // If the statement does not start with a recognized keyword, it is
        // parsed as an expression statement (e.g., an assignment).
        return parseExpressionStatement();
    }
}

std::unique_ptr<StatementNode> Parser::parseExpressionStatement() {
    // The entire line is parsed as a single expression.
    auto expr = parseExpression(Precedence::LOWEST);
    auto stmt = std::make_unique<ExpressionStatementNode>(std::move(expr));

    // Optionally consume a semicolon at the end of the statement.
    if (peek_token_.type == TokenType::SEMICOLON) {
        nextToken();
    }
    return stmt;
}

std::unique_ptr<StatementNode> Parser::parseVarDeclStatement() {
    std::string type = current_token_.value;
    if (!expectPeek(TokenType::IDENTIFIER))
        return nullptr;

    std::string name = current_token_.value;
    if (!expectPeek(TokenType::ASSIGN))
        return nullptr;

    nextToken(); // Move past '=' to the start of the expression.
    auto value = parseExpression(Precedence::LOWEST);

    if (peek_token_.type == TokenType::SEMICOLON) {
        nextToken();
    }

    return std::make_unique<VarDeclNode>(type, name, std::move(value));
}

std::unique_ptr<StatementNode> Parser::parseIfStatement() {
    if (!expectPeek(TokenType::LPAREN))
        return nullptr;
    nextToken(); // Move past '('.
    auto condition = parseExpression(Precedence::LOWEST);
    if (!expectPeek(TokenType::RPAREN))
        return nullptr;
    if (!expectPeek(TokenType::LBRACE))
        return nullptr;

    auto consequence = parseBlockStatement();

    std::unique_ptr<StatementNode> alternative = nullptr;
    // Check for an 'elif' or 'else' branch.
    if (peek_token_.type == TokenType::ELIF) {
        nextToken();
        // An 'elif' is parsed recursively as another 'if' statement.
        alternative = parseIfStatement();
    } else if (peek_token_.type == TokenType::ELSE) {
        nextToken();
        if (!expectPeek(TokenType::LBRACE))
            return nullptr;
        alternative = parseBlockStatement();
    }

    return std::make_unique<IfStatementNode>(std::move(condition), std::move(consequence), std::move(alternative));
}

std::unique_ptr<BlockStatementNode> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatementNode>();
    nextToken(); // Move past '{'.

    while (current_token_.type != TokenType::RBRACE && current_token_.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    return block;
}

std::unique_ptr<ExpressionNode> Parser::parseExpression(Precedence precedence) {
    // The first token of an expression determines its prefix parsing function.
    std::unique_ptr<ExpressionNode> left_exp;
    switch (current_token_.type) {
    case TokenType::IDENTIFIER:
        left_exp = parseIdentifier();
        break;
    case TokenType::INTEGER_LITERAL:
        left_exp = parseIntegerLiteral();
        break;
    case TokenType::TRUE:
    case TokenType::FALSE:
        left_exp = parseBoolean();
        break;
    case TokenType::MINUS:
        left_exp = parsePrefixExpression();
        break;
    default:
        // If no prefix parsing function is found, there is a syntax error.
        return nullptr;
    }

    // After parsing the prefix, look for infix operators as long as their
    // precedence is higher than the current precedence level.
    while (peek_token_.type != TokenType::SEMICOLON &&
           precedence < (precedences_.count(peek_token_.type) ? precedences_[peek_token_.type] : Precedence::LOWEST)) {
        if (!precedences_.count(peek_token_.type)) {
            return left_exp;
        }
        nextToken();
        left_exp = parseInfixExpression(std::move(left_exp));
    }
    return left_exp;
}

std::unique_ptr<ExpressionNode> Parser::parseIdentifier() {
    return std::make_unique<IdentifierNode>(current_token_.value);
}

std::unique_ptr<ExpressionNode> Parser::parseIntegerLiteral() {
    int32_t value = std::stoi(current_token_.value);
    return std::make_unique<NumberLiteralNode>(value);
}

std::unique_ptr<ExpressionNode> Parser::parseBoolean() {
    return std::make_unique<BooleanLiteralNode>(current_token_.type == TokenType::TRUE);
}

std::unique_ptr<ExpressionNode> Parser::parsePrefixExpression() {
    std::string op = current_token_.value;
    nextToken();
    auto right = parseExpression(Precedence::PREFIX);
    return std::make_unique<PrefixExpressionNode>(op, std::move(right));
}

std::unique_ptr<ExpressionNode> Parser::parseInfixExpression(std::unique_ptr<ExpressionNode> left) {
    std::string op = current_token_.value;
    Precedence current_precedence = precedences_[current_token_.type];
    nextToken();
    auto right = parseExpression(current_precedence);
    return std::make_unique<InfixExpressionNode>(std::move(left), op, std::move(right));
}

bool Parser::expectPeek(TokenType type) {
    if (peek_token_.type == type) {
        nextToken();
        return true;
    }
    std::cerr << "Parser Error: Expected next token to be of type " << static_cast<int>(type) << ", got "
              << static_cast<int>(peek_token_.type) << " instead.\n";
    return false;
}

} // namespace suplang