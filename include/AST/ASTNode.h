#pragma once
#include <string>
#include <vector>
#include <memory>

class ExpressionNode;
class StatementNode;

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class ExpressionNode : public ASTNode {};

class StatementNode : public ASTNode {};

class NumberLiteralNode : public ExpressionNode {
public:
    int32_t value;
    NumberLiteralNode(int32_t val) : value(val) {}
};

class VarDeclNode : public StatementNode {
public:
    std::string varType;
    std::string varName;
    std::unique_ptr<ExpressionNode> initialValue;

    VarDeclNode(const std::string& type, const std::string& name, std::unique_ptr<ExpressionNode> value)
        : varType(type), varName(name), initialValue(std::move(value)) {}
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};