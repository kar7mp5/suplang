#include "Interpreter/Interpreter.h"

// This .cpp file needs the full definition of all Object types to perform
// dynamic casts and access member variables.
#include "Object/Object.h"

#include <iostream>

namespace suplang {

namespace {
// Helper function to determine if an object is "truthy" in a boolean context.
// In our language, only the boolean value `false` is falsy.
// Everything else (including the number 0) is considered truthy.
bool IsTruthy(std::shared_ptr<Object> obj) {
    if (!obj)
        return false;
    if (auto b = std::dynamic_pointer_cast<BooleanObject>(obj)) {
        return b->value;
    }
    return true;
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
    if (auto rs = dynamic_cast<ReturnStatementNode *>(node))
        return evalReturnStatement(rs, env);
    if (auto is = dynamic_cast<IfStatementNode *>(node))
        return evalIfStatement(is, env);
    if (auto ws = dynamic_cast<WhileStatementNode *>(node))
        return evalWhileStatement(ws, env);
    if (auto ie = dynamic_cast<InfixExpressionNode *>(node))
        return evalInfixExpression(ie, env);
    if (auto pe = dynamic_cast<PrefixExpressionNode *>(node))
        return evalPrefixExpression(pe, env);

    // Evaluate expressions.
    if (auto nl = dynamic_cast<NumberLiteralNode *>(node)) {
        return std::make_shared<IntegerObject>(nl->value);
    }
    if (auto bl = dynamic_cast<BooleanLiteralNode *>(node)) {
        return std::make_shared<BooleanObject>(bl->value);
    }
    if (auto id = dynamic_cast<IdentifierNode *>(node)) {
        return env->get(id->value);
    }
    if (auto fl = dynamic_cast<FunctionLiteralNode *>(node)) {
        // When a function is defined, capture the current environment `env`.
        // This is how closures work.
        return std::make_shared<FunctionObject>(fl->parameters, std::move(fl->body), env);
    }
    if (auto ce = dynamic_cast<CallExpressionNode *>(node)) {
        // Evaluate the function identifier/literal to get a FunctionObject.
        auto function = eval(ce->function.get(), env);
        if (!function)
            return nullptr;

        // Evaluate all arguments passed to the function.
        std::vector<std::shared_ptr<Object>> args;
        for (const auto &arg_node : ce->arguments) {
            args.push_back(eval(arg_node.get(), env));
        }
        return applyFunction(function, args);
    }

    return nullptr;
}

std::shared_ptr<Object> Interpreter::evalProgram(ProgramNode *node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result;
    for (const auto &stmt : node->statements) {
        result = eval(stmt.get(), env);
        // If a return statement is encountered, stop execution and propagate
        // the return value up.
        if (result && result->type == ObjectType::RETURN_VALUE) {
            return std::dynamic_pointer_cast<ReturnValueObject>(result)->value;
        }
    }
    return result;
}

std::shared_ptr<Object> Interpreter::evalBlockStatement(BlockStatementNode *node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result;
    for (const auto &stmt : node->statements) {
        result = eval(stmt.get(), env);
        // If a return object is found, we must stop evaluation of the block
        // and propagate it upwards.
        if (result && result->type == ObjectType::RETURN_VALUE) {
            return result;
        }
    }
    return result;
}

std::shared_ptr<Object> Interpreter::evalWhileStatement(WhileStatementNode *node, std::shared_ptr<Environment> env) {
    std::shared_ptr<Object> result = nullptr;
    auto condition = eval(node->condition.get(), env);

    while (IsTruthy(condition)) {
        result = eval(node->body.get(), env);
        // If a return statement is executed inside the loop, break out.
        if (result && result->type == ObjectType::RETURN_VALUE) {
            return result;
        }
        // Re-evaluate the condition for the next iteration.
        condition = eval(node->condition.get(), env);
    }
    return result;
}

std::shared_ptr<Object> Interpreter::evalReturnStatement(ReturnStatementNode *node, std::shared_ptr<Environment> env) {
    auto val = eval(node->return_value.get(), env);
    // Wrap the actual return value in a special ReturnValueObject to signal
    // that the function should stop executing.
    return std::make_shared<ReturnValueObject>(val);
}

std::shared_ptr<Object> Interpreter::applyFunction(std::shared_ptr<Object> fn,
                                                   const std::vector<std::shared_ptr<Object>> &args) {
    if (fn->type != ObjectType::FUNCTION) {
        // Handle error: trying to call a non-function.
        return nullptr;
    }
    auto fn_obj = std::dynamic_pointer_cast<FunctionObject>(fn);

    // Create a new, extended environment for the function call.
    auto extended_env = extendFunctionEnv(fn_obj.get(), args);

    // Evaluate the function body within this new, temporary environment.
    auto evaluated = eval(fn_obj->body.get(), extended_env);

    // If the evaluation of the body resulted in a return statement, we
    // "unwrap" the value to get the actual return object.
    if (evaluated && evaluated->type == ObjectType::RETURN_VALUE) {
        return std::dynamic_pointer_cast<ReturnValueObject>(evaluated)->value;
    }
    return evaluated;
}

std::shared_ptr<Environment> Interpreter::extendFunctionEnv(FunctionObject *fn,
                                                            const std::vector<std::shared_ptr<Object>> &args) {
    // Create a new environment that is enclosed by the function's definition
    // environment (`fn->env`). This is crucial for closures.
    auto env = std::make_shared<Environment>(fn->env);

    // Bind the arguments to the parameter names in the new environment.
    for (size_t i = 0; i < fn->parameters.size(); ++i) {
        env->set(fn->parameters[i].param_name, args[i]);
    }
    return env;
}

std::shared_ptr<Object> Interpreter::evalVarDecl(VarDeclNode *node, std::shared_ptr<Environment> env) {
    auto value = eval(node->initialValue.get(), env);
    if (value) {
        env->set(node->varName, value);
    }
    return value;
}

std::shared_ptr<Object> Interpreter::evalIfStatement(IfStatementNode *node, std::shared_ptr<Environment> env) {
    auto condition = eval(node->condition.get(), env);
    if (IsTruthy(condition)) {
        return eval(node->consequence.get(), env);
    } else if (node->alternative) {
        return eval(node->alternative.get(), env);
    }
    return nullptr;
}

std::shared_ptr<Object> Interpreter::evalInfixExpression(InfixExpressionNode *node, std::shared_ptr<Environment> env) {
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
        if (node->op == ">")
            return std::make_shared<BooleanObject>(left_val > right_val);
        if (node->op == "<")
            return std::make_shared<BooleanObject>(left_val < right_val);
        if (node->op == "==")
            return std::make_shared<BooleanObject>(left_val == right_val);
        if (node->op == "!=")
            return std::make_shared<BooleanObject>(left_val != right_val);
    }
    return nullptr;
}

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
    return nullptr;
}

} // namespace suplang