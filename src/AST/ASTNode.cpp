#pragma once
#include <string>
#include <vector>
#include <memory>

class ExpressionNode; // 전방 선언
class StatementNode;

// 모든 AST 노드의 기반이 될 기본 클래스
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// 표현식(Expression) 노드의 기본 클래스
class ExpressionNode : public ASTNode {};

// 문장(Statement) 노드의 기본 클래스
class StatementNode : public ASTNode {};

// 숫자 리터럴을 표현하는 노드 (예: 5)
class NumberLiteralNode : public ExpressionNode {
public:
    int32_t value;
    NumberLiteralNode(int32_t val) : value(val) {}
};

// 변수 선언문을 표현하는 노드 (예: int32 x = 5;)
class VarDeclNode : public StatementNode {
public:
    std::string varType;
    std::string varName;
    std::unique_ptr<ExpressionNode> initialValue;

    VarDeclNode(const std::string& type, const std::string& name, std::unique_ptr<ExpressionNode> value)
        : varType(type), varName(name), initialValue(std::move(value)) {}
};

// 프로그램 전체를 나타내는 루트 노드
class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};