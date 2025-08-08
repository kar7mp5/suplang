#ifndef SUPLANG_AST_ASTNODE_H_
#define SUPLANG_AST_ASTNODE_H_

#include <memory>
#include <string>
#include <vector>

namespace suplang {

// Forward declarations.
class ExpressionNode;
class StatementNode;
class BlockStatementNode;

// Represents a single typed parameter in a function definition.
struct Parameter {
    std::string type_name;
    std::string param_name;
};

// Base class for all nodes in the Abstract Syntax Tree (AST).
class ASTNode {
  public:
    virtual ~ASTNode() = default;
};

// Base class for all nodes that represent an expression.
class ExpressionNode : public ASTNode {};

// Base class for all nodes that represent a statement.
class StatementNode : public ASTNode {};

// Represents a `while` loop statement.
class WhileStatementNode : public StatementNode {
  public:
    WhileStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> body)
        : condition(std::move(cond)), body(std::move(body)) {}

    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> body;
};

// Other existing node definitions...
class FunctionLiteralNode : public ExpressionNode {
  public:
    FunctionLiteralNode(std::vector<Parameter> params, std::unique_ptr<BlockStatementNode> body)
        : parameters(std::move(params)), body(std::move(body)) {}

    std::vector<Parameter> parameters;
    std::unique_ptr<BlockStatementNode> body;
};

class CallExpressionNode : public ExpressionNode {
  public:
    CallExpressionNode(std::unique_ptr<ExpressionNode> func, std::vector<std::unique_ptr<ExpressionNode>> args)
        : function(std::move(func)), arguments(std::move(args)) {}

    std::unique_ptr<ExpressionNode> function;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
};

class ReturnStatementNode : public StatementNode {
  public:
    explicit ReturnStatementNode(std::unique_ptr<ExpressionNode> val) : return_value(std::move(val)) {}
    std::unique_ptr<ExpressionNode> return_value;
};

class NumberLiteralNode : public ExpressionNode {
  public:
    explicit NumberLiteralNode(int32_t val) : value(val) {}
    int32_t value;
};

class BooleanLiteralNode : public ExpressionNode {
  public:
    explicit BooleanLiteralNode(bool val) : value(val) {}
    bool value;
};

class IdentifierNode : public ExpressionNode {
  public:
    explicit IdentifierNode(const std::string &val) : value(val) {}
    std::string value;
};

class PrefixExpressionNode : public ExpressionNode {
  public:
    PrefixExpressionNode(const std::string &op, std::unique_ptr<ExpressionNode> right)
        : op(op), right(std::move(right)) {}
    std::string op;
    std::unique_ptr<ExpressionNode> right;
};

class InfixExpressionNode : public ExpressionNode {
  public:
    InfixExpressionNode(std::unique_ptr<ExpressionNode> left, const std::string &op,
                        std::unique_ptr<ExpressionNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
    std::unique_ptr<ExpressionNode> left;
    std::string op;
    std::unique_ptr<ExpressionNode> right;
};

class ExpressionStatementNode : public StatementNode {
  public:
    explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expr) : expression(std::move(expr)) {}
    std::unique_ptr<ExpressionNode> expression;
};

class BlockStatementNode : public StatementNode {
  public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

class IfStatementNode : public StatementNode {
  public:
    IfStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> cons,
                    std::unique_ptr<StatementNode> alt)
        : condition(std::move(cond)), consequence(std::move(cons)), alternative(std::move(alt)) {}
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> consequence;
    std::unique_ptr<StatementNode> alternative;
};

class VarDeclNode : public StatementNode {
  public:
    VarDeclNode(const std::string &type, const std::string &name, std::unique_ptr<ExpressionNode> value)
        : varType(type), varName(name), initialValue(std::move(value)) {}
    std::string varType;
    std::string varName;
    std::unique_ptr<ExpressionNode> initialValue;
};

class ProgramNode : public ASTNode {
  public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

} // namespace suplang

#endif // SUPLANG_AST_ASTNODE_H_