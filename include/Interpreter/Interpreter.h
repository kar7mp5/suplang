#ifndef SUPLANG_INTERPRETER_INTERPRETER_H_
#define SUPLANG_INTERPRETER_INTERPRETER_H_

#include "AST/ASTNode.h"
#include "Interpreter/Environment.h"

#include <memory>
#include <vector>

namespace suplang {

// Forward declarations to break include cycles. The full definitions will be
// included in the corresponding .cpp file.
class Object;
class FunctionObject;

// The Interpreter class traverses the AST and evaluates it.
class Interpreter {
  public:
    std::shared_ptr<Object> eval(ASTNode *node, std::shared_ptr<Environment> env);

  private:
    // Methods for evaluating specific AST node types.
    std::shared_ptr<Object> evalProgram(ProgramNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalBlockStatement(BlockStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalVarDecl(VarDeclNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalIfStatement(IfStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalWhileStatement(WhileStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalPrefixExpression(PrefixExpressionNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalReturnStatement(ReturnStatementNode *node, std::shared_ptr<Environment> env);
    std::shared_ptr<Object> evalInfixExpression(InfixExpressionNode *node, std::shared_ptr<Environment> env);

    // Helper for applying a function.
    std::shared_ptr<Object> applyFunction(std::shared_ptr<Object> fn, const std::vector<std::shared_ptr<Object>> &args);
    // Helper for creating a function's local environment.
    std::shared_ptr<Environment> extendFunctionEnv(FunctionObject *fn,
                                                   const std::vector<std::shared_ptr<Object>> &args);
};

} // namespace suplang

#endif // SUPLANG_INTERPRETER_INTERPRETER_H_