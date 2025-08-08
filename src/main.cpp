#include <iostream>
#include <memory>
#include <string>

#include "AST/ASTNode.h"
#include "Interpreter/Environment.h"
#include "Interpreter/Interpreter.h"
#include "Lexer/Lexer.h"
#include "Object/Object.h"
#include "Parser/Parser.h"

namespace {
// A utility function to recursively print the AST for debugging purposes.
void PrintAST(const suplang::ASTNode *node, int indent = 0) {
    if (!node) {
        return;
    }

    // Use indentation to represent the tree structure.
    std::cout << std::string(indent * 2, ' ');

    if (auto p = dynamic_cast<const suplang::ProgramNode *>(node)) {
        std::cout << "[Program]\n";
        for (const auto &stmt : p->statements) {
            PrintAST(stmt.get(), indent + 1);
        }
    } else if (auto vd = dynamic_cast<const suplang::VarDeclNode *>(node)) {
        std::cout << "[VarDecl] Type: " << vd->varType << ", Name: " << vd->varName << "\n";
        PrintAST(vd->initialValue.get(), indent + 2);
    } else if (auto es = dynamic_cast<const suplang::ExpressionStatementNode *>(node)) {
        std::cout << "[ExprStmt]\n";
        PrintAST(es->expression.get(), indent + 1);
    } else if (auto rs = dynamic_cast<const suplang::ReturnStatementNode *>(node)) {
        std::cout << "[ReturnStmt]\n";
        PrintAST(rs->return_value.get(), indent + 1);
    } else if (auto ws = dynamic_cast<const suplang::WhileStatementNode *>(node)) {
        std::cout << "[WhileStmt]\n";
        std::cout << std::string((indent + 1) * 2, ' ') << "[Condition]\n";
        PrintAST(ws->condition.get(), indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "[Body]\n";
        PrintAST(ws->body.get(), indent + 2);
    } else if (auto is = dynamic_cast<const suplang::IfStatementNode *>(node)) {
        std::cout << "[IfStmt]\n";
        std::cout << std::string((indent + 1) * 2, ' ') << "[Condition]\n";
        PrintAST(is->condition.get(), indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "[Consequence]\n";
        PrintAST(is->consequence.get(), indent + 2);
        if (is->alternative) {
            std::cout << std::string((indent + 1) * 2, ' ') << "[Alternative]\n";
            PrintAST(is->alternative.get(), indent + 2);
        }
    } else if (auto bs = dynamic_cast<const suplang::BlockStatementNode *>(node)) {
        std::cout << "[BlockStmt]\n";
        for (const auto &stmt : bs->statements) {
            PrintAST(stmt.get(), indent + 1);
        }
    } else if (auto fl = dynamic_cast<const suplang::FunctionLiteralNode *>(node)) {
        std::cout << "[FunctionLiteral]\n";
        std::cout << std::string((indent + 1) * 2, ' ') << "[Parameters]\n";
        for (const auto &param : fl->parameters) {
            std::cout << std::string((indent + 2) * 2, ' ') << param.type_name << " " << param.param_name << "\n";
        }
        std::cout << std::string((indent + 1) * 2, ' ') << "[Body]\n";
        PrintAST(fl->body.get(), indent + 2);
    } else if (auto ce = dynamic_cast<const suplang::CallExpressionNode *>(node)) {
        std::cout << "[CallExpr]\n";
        std::cout << std::string((indent + 1) * 2, ' ') << "[Function]\n";
        PrintAST(ce->function.get(), indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "[Arguments]\n";
        for (const auto &arg : ce->arguments) {
            PrintAST(arg.get(), indent + 2);
        }
    } else if (auto ie = dynamic_cast<const suplang::InfixExpressionNode *>(node)) {
        std::cout << "[InfixExpr] Op: " << ie->op << "\n";
        PrintAST(ie->left.get(), indent + 1);
        PrintAST(ie->right.get(), indent + 1);
    } else if (auto id = dynamic_cast<const suplang::IdentifierNode *>(node)) {
        std::cout << "[Identifier] " << id->value << "\n";
    } else if (auto nl = dynamic_cast<const suplang::NumberLiteralNode *>(node)) {
        std::cout << "[Number] " << nl->value << "\n";
    } else if (auto bl = dynamic_cast<const suplang::BooleanLiteralNode *>(node)) {
        std::cout << "[Boolean] " << (bl->value ? "true" : "false") << "\n";
    }
}
} // namespace

int main() {
    // The source code to be interpreted.
    std::string code = R"(
      int32 counter = 0;
      while (counter < 3) {
          counter = counter + 1;
      }
      int32 result = counter;
  )";

    // 1. Lexing
    suplang::Lexer lexer(code);

    // 2. Parsing
    suplang::Parser parser(lexer);
    auto ast = parser.parseProgram();

    // 3. Print the AST for debugging.
    std::cout << "--- Abstract Syntax Tree ---\n";
    PrintAST(ast.get());
    std::cout << "--------------------------\n\n";

    // 4. Interpreting
    suplang::Interpreter interpreter;
    auto env = std::make_shared<suplang::Environment>();
    interpreter.eval(ast.get(), env);

    std::cout << code << std::endl;

    // --- Verification ---
    std::cout << "--- Execution Result ---\n";
    auto result_obj = env->get("result");
    if (result_obj && result_obj->type == suplang::ObjectType::INTEGER) {
        auto int_obj = std::dynamic_pointer_cast<suplang::IntegerObject>(result_obj);
        std::cout << "Variable 'result' has value: " << int_obj->value << std::endl;
    } else {
        std::cout << "Variable 'result' not found or not an integer." << std::endl;
    }

    return 0;
}