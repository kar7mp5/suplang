#include "Interpreter/Interpreter.h"

#include <iostream>

namespace suplang {

namespace {
// Helper function to determine if an object is "truthy" in a boolean context.
bool IsTruthy(std::shared_ptr<Object> obj) {
    if (!obj)
        return false;
    if (auto b = std::dynamic_pointer_cast<BooleanObject>(obj)) {
        return b->value;
    }
    // In the future, you might consider 0, empty strings, etc., as false.
    return false;
}
} // namespace

// The main dispatch function for evaluation. It uses dynamic_cast to
// determine the node type and call the appropriate evaluation method.
std::shared_ptr<Object> Interpreter::eval(ASTNode *node, std::shared_ptr<Environment> env) {
    if (!node)
        return nullptr;

    if (auto p = dynamic_cast<ProgramNode *>(node))
        return evalProgram(p, env);
    if (auto bs = dynamic_cast<BlockStatementNode *>(node))
        return evalBlockStatement(bs, env);
    if (auto es = dynamic_cast<ExpressionStatementNode *>(node))
        return eval(es->expression.get(), env);
    if (auto vd = dynamic_cast<VarDeclNode *>(node))
        return evalVarDecl(vd, env);
    if (auto is = dynamic_cast<IfStatementNode *>(node))
        return evalIfStatement(is, env);
    if (auto ie = dynamic_cast<InfixExpressionNode *>(node))
        return evalInfixExpression(ie, env);
    if (auto pe = dynamic_cast<PrefixExpressionNode *>(node))
        return evalPrefixExpression(pe, env);

    // Evaluate literals and identifiers.
    if (auto nl = dynamic_cast<NumberLiteralNode *>(node)) {
        return std::make_shared<IntegerObject>(nl->value);
    }
    if (auto bl = dynamic_cast<BooleanLiteralNode *>(node)) {
        return std::make_shared<BooleanObject>(bl->value);
    }
    if (auto id = dynamic_cast<IdentifierNode *>(node)) {
        return env->get(id->value);
    }

    return nullptr;
}

// Evaluates a program by evaluating each statement in sequence.
std::shared_ptr<Object> Interpreter::evalProgram(ProgramNode *node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result;
    for (const auto &stmt : node->statements) {
        result = eval(stmt.get(), env);
        // TODO(user): Add logic here for 'return' statements to halt execution.
    }
    return result; // Return the result of the last statement.
}

// Evaluates a block of statements.
std::shared_ptr<Object> Interpreter::evalBlockStatement(BlockStatementNode *node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result;
    for (const auto &stmt : node->statements) {
        result = eval(stmt.get(), env);
        // TODO(user): Add logic here for 'return' statements.
    }
    return result;
}

// Evaluates a variable declaration statement.
std::shared_ptr<Object> Interpreter::evalVarDecl(VarDeclNode *node, std::shared_ptr<Environment> env) {
    // First, evaluate the initializer expression to get the value.
    auto value = eval(node->initialValue.get(), env);
    if (value) {
        // Then, store the variable and its evaluated value in the environment.
        env->set(node->varName, value);
    }
    return value;
}

// Evaluates an if-elif-else statement.
std::shared_ptr<Object> Interpreter::evalIfStatement(IfStatementNode *node, std::shared_ptr<Environment> env) {
    auto condition = eval(node->condition.get(), env);
    if (IsTruthy(condition)) {
        return eval(node->consequence.get(), env);
    } else if (node->alternative) {
        return eval(node->alternative.get(), env);
    }
    // No result if condition is false and there's no else/elif branch.
    return nullptr;
}

// Evaluates an infix expression, e.g., 5 + 10.
std::shared_ptr<Object> Interpreter::evalInfixExpression(InfixExpressionNode *node, std::shared_ptr<Environment> env) {
    // Special case for assignment, which does not evaluate the left side.
    if (node->op == "=") {
        auto right_val = eval(node->right.get(), env);
        if (auto id = dynamic_cast<IdentifierNode *>(node->left.get())) {
            env->set(id->value, right_val);
            return right_val;
        }
    }

    auto left = eval(node->left.get(), env);
    auto right = eval(node->right.get(), env);

    if (!left || !right)
        return nullptr;

    // Currently only supports integer arithmetic and comparison.
    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        auto left_val = std::dynamic_pointer_cast<IntegerObject>(left)->value;
        auto right_val = std::dynamic_pointer_cast<IntegerObject>(right)->value;

        if (node->op == "+")
            return std::make_shared<IntegerObject>(left_val + right_val);
        if (node->op == "-")
            return std::make_shared<IntegerObject>(left_val - right_val);
        if (node->op == "*")
            return std::make_shared<IntegerObject>(left_val * right_val);
        if (node->op == "/")
            return std::make_shared<IntegerObject>(left_val / right_val);

        // Comparison operators return Boolean objects.
        if (node->op == ">")
            return std::make_shared<BooleanObject>(left_val > right_val);
        if (node->op == "<")
            return std::make_shared<BooleanObject>(left_val < right_val);
        if (node->op == "==")
            return std::make_shared<BooleanObject>(left_val == right_val);
        if (node->op == "!=")
            return std::make_shared<BooleanObject>(left_val != right_val);
    }

    return nullptr; // Return null if the operation is not supported.
}

// Evaluates a prefix expression, e.g., -15.
std::shared_ptr<Object> Interpreter::evalPrefixExpression(PrefixExpressionNode *node,
                                                          std::shared_ptr<Environment> env) {
    auto right = eval(node->right.get(), env);
    if (!right)
        return nullptr;

    if (node->op == "-") {
        if (right->type != ObjectType::INTEGER)
            return nullptr;
        auto val = std::dynamic_pointer_cast<IntegerObject>(right)->value;
        return std::make_shared<IntegerObject>(-val);
    }

    // TODO(user): Add '!' logical not operator.

    return nullptr; // Return null for unknown prefix operators.
}

} // namespace suplang