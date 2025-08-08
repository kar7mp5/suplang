#include "Parser/Parser.h"

#include <iostream>

namespace suplang {

Parser::Parser(Lexer &lexer) : lexer_(lexer) {
    // Sets up the precedence table for infix operators.
    precedences_ = {
        {TokenType::ASSIGN, Precedence::EQUALS},     {TokenType::EQUALS, Precedence::EQUALS},
        {TokenType::NOT_EQUALS, Precedence::EQUALS}, {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},    {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},         {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},  {TokenType::LPAREN, Precedence::CALL},
    };

    // Initializes the parser by reading the first two tokens.
    nextToken();
    nextToken();
}

// Consumes the current token and advances the lexer to the next one.
void Parser::nextToken() {
    current_token_ = peek_token_;
    peek_token_ = lexer_.nextToken();
}

// Asserts the type of the next token and advances if it matches.
bool Parser::expectPeek(TokenType type) {
    if (peek_token_.type == type) {
        nextToken();
        return true;
    }
    std::cerr << "Parser Error: Expected next token to be of type " << static_cast<int>(type) << ", got "
              << static_cast<int>(peek_token_.type) << " instead.\n";
    return false;
}

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();
    while (current_token_.type != TokenType::END_OF_FILE) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    return program;
}

std::unique_ptr<StatementNode> Parser::parseStatement() {
    switch (current_token_.type) {
    case TokenType::INT32:
    case TokenType::BOOL:
        return parseVarDeclStatement();
    case TokenType::IF:
        return parseIfStatement();
    case TokenType::WHILE:
        return parseWhileStatement();
    case TokenType::RETURN:
        return parseReturnStatement();
    default:
        return parseExpressionStatement();
    }
}

std::unique_ptr<StatementNode> Parser::parseWhileStatement() {
    if (!expectPeek(TokenType::LPAREN))
        return nullptr;
    nextToken();
    auto condition = parseExpression(Precedence::LOWEST);
    if (!expectPeek(TokenType::RPAREN))
        return nullptr;
    if (!expectPeek(TokenType::LBRACE))
        return nullptr;
    auto body = parseBlockStatement();
    return std::make_unique<WhileStatementNode>(std::move(condition), std::move(body));
}

std::unique_ptr<StatementNode> Parser::parseExpressionStatement() {
    auto expr = parseExpression(Precedence::LOWEST);
    auto stmt = std::make_unique<ExpressionStatementNode>(std::move(expr));
    if (peek_token_.type == TokenType::SEMICOLON) {
        nextToken();
    }
    return stmt;
}

std::unique_ptr<StatementNode> Parser::parseReturnStatement() {
    nextToken();
    auto return_value = parseExpression(Precedence::LOWEST);
    if (peek_token_.type == TokenType::SEMICOLON) {
        nextToken();
    }
    return std::make_unique<ReturnStatementNode>(std::move(return_value));
}

std::unique_ptr<StatementNode> Parser::parseVarDeclStatement() {
    std::string type = current_token_.value;
    if (!expectPeek(TokenType::IDENTIFIER))
        return nullptr;
    std::string name = current_token_.value;
    if (!expectPeek(TokenType::ASSIGN))
        return nullptr;
    nextToken();
    auto value = parseExpression(Precedence::LOWEST);
    if (peek_token_.type == TokenType::SEMICOLON) {
        nextToken();
    }
    return std::make_unique<VarDeclNode>(type, name, std::move(value));
}

std::unique_ptr<StatementNode> Parser::parseIfStatement() {
    if (!expectPeek(TokenType::LPAREN))
        return nullptr;
    nextToken();
    auto condition = parseExpression(Precedence::LOWEST);
    if (!expectPeek(TokenType::RPAREN))
        return nullptr;
    if (!expectPeek(TokenType::LBRACE))
        return nullptr;
    auto consequence = parseBlockStatement();
    std::unique_ptr<StatementNode> alternative = nullptr;
    if (peek_token_.type == TokenType::ELIF) {
        nextToken();
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
    nextToken();
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
    case TokenType::DEF:
        left_exp = parseFunctionLiteral();
        break;
    default:
        return nullptr;
    }

    while (peek_token_.type != TokenType::SEMICOLON &&
           precedence <
               (precedences_.count(peek_token_.type) ? precedences_.at(peek_token_.type) : Precedence::LOWEST)) {
        if (peek_token_.type == TokenType::LPAREN) {
            nextToken();
            left_exp = parseCallExpression(std::move(left_exp));
        } else if (precedences_.count(peek_token_.type)) {
            nextToken();
            left_exp = parseInfixExpression(std::move(left_exp));
        } else {
            return left_exp;
        }
    }
    return left_exp;
}

std::unique_ptr<ExpressionNode> Parser::parseFunctionLiteral() {
    if (!expectPeek(TokenType::IDENTIFIER))
        return nullptr;
    if (!expectPeek(TokenType::LPAREN))
        return nullptr;
    auto params = parseFunctionParameters();
    if (!expectPeek(TokenType::LBRACE))
        return nullptr;
    auto body = parseBlockStatement();
    return std::make_unique<FunctionLiteralNode>(std::move(params), std::move(body));
}

std::vector<Parameter> Parser::parseFunctionParameters() {
    std::vector<Parameter> params;
    if (peek_token_.type == TokenType::RPAREN) {
        nextToken();
        return params;
    }
    nextToken();
    Parameter p = {current_token_.value, ""};
    if (!expectPeek(TokenType::IDENTIFIER))
        return {};
    p.param_name = current_token_.value;
    params.push_back(p);
    while (peek_token_.type == TokenType::COMMA) {
        nextToken();
        nextToken();
        Parameter p2 = {current_token_.value, ""};
        if (!expectPeek(TokenType::IDENTIFIER))
            return {};
        p2.param_name = current_token_.value;
        params.push_back(p2);
    }
    if (!expectPeek(TokenType::RPAREN))
        return {};
    return params;
}

std::unique_ptr<ExpressionNode> Parser::parseCallExpression(std::unique_ptr<ExpressionNode> function) {
    auto args = parseCallArguments();
    return std::make_unique<CallExpressionNode>(std::move(function), std::move(args));
}

std::vector<std::unique_ptr<ExpressionNode>> Parser::parseCallArguments() {
    std::vector<std::unique_ptr<ExpressionNode>> args;
    if (peek_token_.type == TokenType::RPAREN) {
        nextToken();
        return args;
    }
    nextToken();
    args.push_back(parseExpression(Precedence::LOWEST));
    while (peek_token_.type == TokenType::COMMA) {
        nextToken();
        nextToken();
        args.push_back(parseExpression(Precedence::LOWEST));
    }
    if (!expectPeek(TokenType::RPAREN))
        return {};
    return args;
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

} // namespace suplang