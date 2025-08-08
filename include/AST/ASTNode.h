#ifndef SUPLANG_AST_ASTNODE_H_
#define SUPLANG_AST_ASTNODE_H_

#include <memory>
#include <string>
#include <vector>

namespace suplang {

class ExpressionNode;
class StatementNode;

// Base class for all nodes in the Abstract Syntax Tree (AST).
// Provides a virtual destructor for safe polymorphic deletion.
class ASTNode {
  public:
    virtual ~ASTNode() = default;
};

// Base class for all nodes that represent an expression.
class ExpressionNode : public ASTNode {};

// Base class for all nodes that represent a statement.
class StatementNode : public ASTNode {};

// Represents a numeric literal expression, e.g., 5.
class NumberLiteralNode : public ExpressionNode {
  public:
    explicit NumberLiteralNode(int32_t val) : value(val) {}
    int32_t value;
};

// Represents a boolean literal, i.e., 'true' or 'false'.
class BooleanLiteralNode : public ExpressionNode {
  public:
    explicit BooleanLiteralNode(bool val) : value(val) {}
    bool value;
};

// Represents an identifier, e.g., a variable name.
class IdentifierNode : public ExpressionNode {
  public:
    explicit IdentifierNode(const std::string &val) : value(val) {}
    std::string value;
};

// Represents a prefix expression, e.g., -15 or !true.
class PrefixExpressionNode : public ExpressionNode {
  public:
    PrefixExpressionNode(const std::string &op, std::unique_ptr<ExpressionNode> right)
        : op(op), right(std::move(right)) {}

    std::string op;
    std::unique_ptr<ExpressionNode> right;
};

// Represents an infix expression, e.g., left + right.
class InfixExpressionNode : public ExpressionNode {
  public:
    InfixExpressionNode(std::unique_ptr<ExpressionNode> left, const std::string &op,
                        std::unique_ptr<ExpressionNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    std::unique_ptr<ExpressionNode> left;
    std::string op;
    std::unique_ptr<ExpressionNode> right;
};

// Represents a statement that consists of a single expression, e.g., x = 5;
class ExpressionStatementNode : public StatementNode {
  public:
    explicit ExpressionStatementNode(std::unique_ptr<ExpressionNode> expr) : expression(std::move(expr)) {}
    std::unique_ptr<ExpressionNode> expression;
};

// Represents a block of statements enclosed in curly braces {}.
class BlockStatementNode : public StatementNode {
  public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

// Represents an if-elif-else conditional statement.
class IfStatementNode : public StatementNode {
  public:
    IfStatementNode(std::unique_ptr<ExpressionNode> cond, std::unique_ptr<BlockStatementNode> cons,
                    std::unique_ptr<StatementNode> alt)
        : condition(std::move(cond)), consequence(std::move(cons)), alternative(std::move(alt)) {}

    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockStatementNode> consequence;
    // Alternative can be another IfStatementNode (for elif) or a
    // BlockStatementNode (for else).
    std::unique_ptr<StatementNode> alternative;
};

// Represents a variable declaration statement, e.g., int32 x = 5;
class VarDeclNode : public StatementNode {
  public:
    VarDeclNode(const std::string &type, const std::string &name, std::unique_ptr<ExpressionNode> value)
        : varType(type), varName(name), initialValue(std::move(value)) {}

    std::string varType;
    std::string varName;
    std::unique_ptr<ExpressionNode> initialValue;
};

// Represents the root of the AST, containing a sequence of statements.
class ProgramNode : public ASTNode {
  public:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

} // namespace suplang

#endif // SUPLANG_AST_ASTNODE_H_