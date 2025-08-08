#ifndef SUPLANG_INTERPRETER_INTERPRETER_H_
#define SUPLANG_INTERPRETER_INTERPRETER_H_

#include "AST/ASTNode.h"
#include "Interpreter/Environment.h"

#include <memory>

namespace suplang {

// The Interpreter class traverses the Abstract Syntax Tree (AST) and
// evaluates it, producing a result.
class Interpreter {
  public:
    // The main dispatch function for evaluation. It takes an AST node and an
    // environment and returns the evaluated object.
    std::shared_ptr<Object> eval(ASTNode *node, std::shared_ptr<Environment> env);

  private:
    std::shared_ptr<Object> evalProgram(ProgramNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalBlockStatement(BlockStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalVarDecl(VarDeclNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalIfStatement(IfStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalInfixExpression(InfixExpressionNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalPrefixExpression(PrefixExpressionNode *node, std::shared_ptr<Environment> env);
};

} // namespace suplang

#endif // SUPLANG_INTERPRETER_INTERPRETER_H_