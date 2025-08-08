#pragma once
#include "AST/ASTNode.h"
#include "Interpreter/Environment.h"

// The interpreter class walks the AST and evaluates it
class Interpreter {
public:
    std::shared_ptr<Object> eval(ASTNode* node, std::shared_ptr<Environment> env);

private:
    std::shared_ptr<Object> evalProgram(ProgramNode* node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalVarDecl(VarDeclNode* node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalInfixExpression(InfixExpressionNode* node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalPrefixExpression(PrefixExpressionNode* node, std::shared_ptr<Environment> env);
};