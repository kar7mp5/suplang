#include "Interpreter/Interpreter.h"
#include <iostream>

// Main dispatch function for evaluation
std::shared_ptr<Object> Interpreter::eval(ASTNode* node, std::shared_ptr<Environment> env) {
    if (!node) return nullptr;

    if (auto p = dynamic_cast<ProgramNode*>(node)) return evalProgram(p, env);
    if (auto vd = dynamic_cast<VarDeclNode*>(node)) return evalVarDecl(vd, env);
    if (auto ie = dynamic_cast<InfixExpressionNode*>(node)) return evalInfixExpression(ie, env);
    if (auto pe = dynamic_cast<PrefixExpressionNode*>(node)) return evalPrefixExpression(pe, env);
    
    // Evaluate literals and identifiers
    if (auto nl = dynamic_cast<NumberLiteralNode*>(node)) {
        return std::make_shared<IntegerObject>(nl->value);
    }
    if (auto id = dynamic_cast<IdentifierNode*>(node)) {
        return env->get(id->value);
    }
    return nullptr;
}

// Evaluates a program by evaluating each statement
std::shared_ptr<Object> Interpreter::evalProgram(ProgramNode* node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result;
    for (const auto& stmt : node->statements) {
        result = eval(stmt.get(), env);
    }
    return result; // Return the result of the last statement
}

// Evaluates a variable declaration statement
std::shared_ptr<Object> Interpreter::evalVarDecl(VarDeclNode* node, std::shared_ptr<Environment> env) {
    // Evaluate the initializer expression
    auto value = eval(node->initialValue.get(), env);
    if (value) {
        // Store the variable and its value in the environment
        env->set(node->varName, value);
    }
    return value;
}

// Evaluates an infix expression (e.g., 5 + 10)
std::shared_ptr<Object> Interpreter::evalInfixExpression(InfixExpressionNode* node, std::shared_ptr<Environment> env) {
    auto left = eval(node->left.get(), env);
    auto right = eval(node->right.get(), env);

    if (!left || !right) return nullptr;

    // Currently only supports integer arithmetic
    if (left->type == ObjectType::INTEGER && right->type == ObjectType::INTEGER) {
        auto leftVal = std::dynamic_pointer_cast<IntegerObject>(left)->value;
        auto rightVal = std::dynamic_pointer_cast<IntegerObject>(right)->value;

        if (node->op == "+") return std::make_shared<IntegerObject>(leftVal + rightVal);
        if (node->op == "-") return std::make_shared<IntegerObject>(leftVal - rightVal);
        if (node->op == "*") return std::make_shared<IntegerObject>(leftVal * rightVal);
        if (node->op == "/") return std::make_shared<IntegerObject>(leftVal / rightVal);
    }

    return nullptr; // Return null if operation is not supported
}

// Evaluates a prefix expression (e.g., -15)
std::shared_ptr<Object> Interpreter::evalPrefixExpression(PrefixExpressionNode* node, std::shared_ptr<Environment> env) {
    auto right = eval(node->right.get(), env);
    if (!right) return nullptr;

    if (node->op == "-") {
        if (right->type != ObjectType::INTEGER) return nullptr;
        auto val = std::dynamic_pointer_cast<IntegerObject>(right)->value;
        return std::make_shared<IntegerObject>(-val);
    }
    return nullptr; // Return null for unknown prefix operators
}