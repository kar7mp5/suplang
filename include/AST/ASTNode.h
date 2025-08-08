#pragma once
#include <string>
#include <vector>
#include <memory>

class ExpressionNode;
class StatementNode;

// Base class for all nodes in the Abstract Syntax Tree
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Base class for all expression nodes
class ExpressionNode : public ASTNode {};

// Base class for all statement nodes
class StatementNode : public ASTNode {};

// Represents a numeric literal, e.g., 5
class NumberLiteralNode : public ExpressionNode {
public:
    int32_t value;
    NumberLiteralNode(int32_t val) : value(val) {}
};

// Represents a variable declaration, e.g., int32 x = 5;
class VarDeclNode : public StatementNode {
public:
    std::string varType;
    std::string varName;
    std::unique_ptr<ExpressionNode> initialValue;

    VarDeclNode(const std::string& type, const std::string& name, std::unique_ptr<ExpressionNode> value)
        : varType(type), varName(name), initialValue(std::move(value)) {}
};

// Represents an identifier (variable name)
class IdentifierNode : public ExpressionNode {
public:
    std::string value;
    IdentifierNode(const std::string& val) : value(val) {}
};

// Represents a prefix expression, e.g., -15
class PrefixExpressionNode : public ExpressionNode {
public:
    std::string op;
    std::unique_ptr<ExpressionNode> right;

    PrefixExpressionNode(const std::string& op, std::unique_ptr<ExpressionNode> right)
        : op(op), right(std::move(right)) {}
};

// Represents an infix expression, e.g., 5 + 10
class InfixExpressionNode : public ExpressionNode {
public:
    std::unique_ptr<ExpressionNode> left;
    std::string op;
    std::unique_ptr<ExpressionNode> right;

    InfixExpressionNode(std::unique_ptr<ExpressionNode> left, const std::string& op, std::unique_ptr<ExpressionNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
};

// Represents the root of the program
class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};